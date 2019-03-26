/*
 * Copyright (C) 2019  CERN for the benefit of the LHCb collaboration
 * Author: Paul Seyfert <pseyfert@cern.ch>
 *
 * This software is distributed under the terms of the GNU General Public
 * Licence version 3 (GPL Version 3), copied verbatim in the file "LICENSE".
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

// functional algorithm things
#include "GaudiAlg/Producer.h"
#include "GaudiAlg/Transformer.h"
#include <tuple>

// zipping
#include "SOAExtensions/ZipContainer.h"

// selections
#include "SOAExtensions/ZipSelection.h"

// local example classes
#include "classes.h"

namespace LHCb {
  namespace Examples {
    using BaseClass_t = Gaudi::Functional::Traits::BaseClass_t<::Algorithm>;

    // Writing to and reading from TES shouldn't be done with zips, but rather broken down.
    // (technical tl;dr: what gets written as zip cannot be read in parts;
    // writing parts and zip simultaneously doesn't work because the zip is a
    // view and return from operator() moves things in memory).

    // This is hard enough to read as it is, so after automatic formatting I adjust to "one return per line".
    // clang-format off
    using HarvesterOutput =
        std::tuple<Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>,
                   Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>,
                   Zipping::ExportedSelection<>>;
    // clang-format on

    struct Harvester final : Gaudi::Functional::MultiTransformer<HarvesterOutput( const int& )> {
      Harvester( const std::string& name, ISvcLocator* svcLoc )
          : MultiTransformer( name, svcLoc, {KeyValue{"Seed", "/Event/MyInt"}},
                              {KeyValue{"Apple_Output", "/Event/Apples"},
                               KeyValue{"Nutrient_Output", "/Event/Nutrients"},
                               KeyValue{"Selection_Output", "/Event/HarvesterSelection"}} ) {}

      mutable Gaudi::Accumulators::AveragingCounter<int>   m_harvested{this, "picked heavy apples"};
      mutable Gaudi::Accumulators::AveragingCounter<float> m_weights{this, "apple weights"};

      HarvesterOutput operator()( const int& seed ) const override {
        // Despite not zipping for at least another 10 lines, we try to keep the same container API for producers and
        // consumers.
        Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>              part1;
        Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>> part2(
            part1.zipIdentifier() ); // assign family number at construction!

        for ( int i = 0; i < 42; ++i ) {
          // Sadly push_back/emplace_back cannot be used on zips at the moment.
          // I'd prefer `auto apples = zip(part1,part2); apples.push_back(....);
          // It would work if we didn't intend to write the components separately to TES
          part1.push_back( ::Examples::Apple{0.2f + 0.001f * ( seed + i )} ); // an apple weighs 200g plus a bit more
          part2.push_back( ::Examples::NutritionalContent{20.f, 500} );
        }

        auto apples = Zipping::semantic_zip<::Examples::Harvester_OutputSkin>( part1, part2 );
        if ( true ) { // imagine "if (do_monitoring)"
          auto buf1 = m_harvested.buffer();
          auto buf2 = m_weights.buffer();
          // yes, there are STL algorithm alternatives for the following
          for ( auto apple : apples ) {
            if ( apple.weight() > 0.210f ) buf1 += 1;
            buf2 += apple.weight();
          }
        }

        Zipping::ExportedSelection<> trivial_selection{apples, Zipping::details::alwaysTrue{}};
        HarvesterOutput              retval =
            std::make_tuple( std::move( part1 ), std::move( part2 ), std::move( trivial_selection ) );
        return retval;
      }
    };
    DECLARE_COMPONENT( Harvester )

    struct Sorter final
        : Gaudi::Functional::Transformer<Zipping::ExportedSelection<>(
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>&,
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>&,
              const Zipping::ExportedSelection<>& )> {
      Gaudi::Property<float> m_minWeight{this, "minWeight", 0.225f, "minimum apple weight"};

      Sorter( const std::string& name, ISvcLocator* svcLoc )
          : Transformer( name, svcLoc,
                         {KeyValue{"Apple_Input", "/Event/Apples"}, KeyValue{"Nutrient_Input", "/Event/Nutrients"},
                          KeyValue{"Selection_Input", "/Event/HarvesterSelection"}},
                         KeyValue{"Selection_Output", "/Event/SorterSelection"} ) {}
      Zipping::ExportedSelection<>
      operator()( const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>&              p1,
                  const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>& p2,
                  const Zipping::ExportedSelection<>& e ) const override {
        auto z = Zipping::semantic_zip<::Examples::Harvester_OutputSkin>( p1, p2 );

        Zipping::SelectionView<decltype( z )> apple_input( &z, e );

        auto refined_exported_selection =
            apple_input.select( [&]( auto apple ) { return apple.weight() > m_minWeight; } );

        {
          size_t i      = 0;
          auto   buffer = m_npassed_cuts.buffer();
          for ( ; i < refined_exported_selection.size(); ++i ) buffer += true;
          for ( ; i < apple_input.size(); ++i ) buffer += false;
        }

        return refined_exported_selection;
      }

      mutable Gaudi::Accumulators::BinomialCounter<> m_npassed_cuts{this, "apple acceptance efficiency"};
    };
    DECLARE_COMPONENT( Sorter )

    using RaterOutput = std::tuple<Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::MarketValueSkin>>,
                                   Zipping::ExportedSelection<>>;

    struct Rater final
        : Gaudi::Functional::MultiTransformer<RaterOutput(
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>&,
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>&,
              const Zipping::ExportedSelection<>& )> {

      Rater( const std::string& name, ISvcLocator* svcLoc )
          : MultiTransformer(
                name, svcLoc,
                // clang-format off
                {KeyValue{"Apple_Input", "/Event/Apples"},
                 KeyValue{"Nutrient_Input", "/Event/Nutrients"},
                 KeyValue{"Selection_Input", "/Event/SorterSelection"}},
                {KeyValue{"Ratings", "/Event/RaterRatings"},
                 KeyValue{"Selection_Output", "/Event/RaterSelection"}} ) {}
      // clang-format on

      RaterOutput
      operator()( const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>&              p1,
                  const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>& p2,
                  const Zipping::ExportedSelection<>& e ) const override {
        auto all_input = Zipping::semantic_zip<::Examples::Harvester_OutputSkin>( p1, p2 );

        ////// would violate constness of views
        // Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::MarketValueSkin>> retval(
        //     all_input.zipIdentifier(), all_input.size(),
        //     ::Examples::MarketValue{} ); // the output shall be zip compatible.
        //
        // auto wide_zip = Zipping::semantic_zip<::Examples::FullAppleSkin>( all_input, retval );
        // Zipping::SelectionView selected_elements( &wide_zip, e );
        //
        // for (auto apple: wide_zip) {
        //   if (selected_elements.end() != std::find(selected_elements.begin(), selected_elements.end(), apple) ) {
        //     apple.marketValue().how_good_it_looks_to_the_consumer = 10.; // every apple is a top quality
        //     apple!
        //   } else {
        //     apple.marketValue().how_good_it_looks_to_the_consumer = 0.; // padding
        //   }
        // }

        Zipping::SelectionView                                                          selected_input( &all_input, e );
        Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::MarketValueSkin>> retval(
            all_input.zipIdentifier() );

        // yeah, this isn't pretty.
        // FIXME: version with std::find segfaults
        for ( auto apple : all_input ) {
          // bool evaluate = false;
          // for ( auto test : selected_input ) {
          //   if ( apple == test ) evaluate = true;
          // }
          bool evaluate = ( selected_input.end() == std::find( selected_input.begin(), selected_input.end(), apple ) );

          if ( evaluate ) {
            retval.emplace_back( 10.f ); // top apple!
          } else {
            retval.emplace_back( 0.f ); // bad apple
          }
        }

        Zipping::ExportedSelection<> trivial_selection( all_input, Zipping::details::alwaysTrue{} );
        return std::make_tuple( std::move( retval ), std::move( trivial_selection ) );
      }
    };
    DECLARE_COMPONENT( Rater )

    using KitchenOutput = std::tuple<Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::FruitSaladSkin>>,
                                     Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::DishSkin>>,
                                     Zipping::ExportedSelection<>>;
    struct Cook final
        : Gaudi::Functional::MultiTransformer<KitchenOutput(
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>&,
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>&,
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::MarketValueSkin>>&,
              const Zipping::ExportedSelection<>& )> {

      Cook( const std::string& name, ISvcLocator* svcLoc )
          : MultiTransformer( name, svcLoc,
                              // clang-format off
                {KeyValue{"Apple_Input", "/Event/Apples"},
                 KeyValue{"Nutrient_Input", "/Event/Nutrients"},
                 KeyValue{"Ratings", "/Event/RaterRatings"},
                 KeyValue{"Selection_Input", "/Event/SorterSelection"}},
                {KeyValue{"Salad_Output", "/Event/CookSalad"},
                 KeyValue{"Dish_Output", "/Event/CookDish"},
                 KeyValue{"Selection_Output", "/Event/CookSelection"}} ) {}
      // clang-format on

      KitchenOutput operator()(
          const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>& in1,
          [[maybe_unused]] const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>&
                                                                                                 in2,
          const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::MarketValueSkin>>& in3,
          const Zipping::ExportedSelection<>&                                                    s ) const override {

        Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::FruitSaladSkin>> out1;
        Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::DishSkin>>       out2( out1.zipIdentifier() );

        auto tmp   = Zipping::semantic_zip<::Examples::StoreSkin>( in1, in3 );
        auto input = Zipping::SelectionView( &tmp, s );

        for ( size_t apple_index_1 = 0; apple_index_1 < input.size(); apple_index_1++ ) {
          for ( size_t apple_index_2 = apple_index_1 + 1; apple_index_2 < input.size(); apple_index_2++ ) {
            auto apple1 = input[apple_index_1];
            auto apple2 = input[apple_index_2];
            // fruit salad must have a minimal weight
            if ( apple1.weight() + apple2.weight() > 0.205f ) {
              ::Examples::FruitSalad push_value1{std::vector<size_t>{apple_index_1, apple_index_2}};
              ::Examples::Dish       push_value2{apple1.apple().weight() + apple2.apple().weight(),
                                           500.f}; // the label in the store just lies about the calories
              out1.push_back( push_value1 );
              out2.push_back( push_value2 );
            }
          }
        }

        Zipping::ExportedSelection<> trivial_selection( out1, Zipping::details::alwaysTrue{} );
        return std::make_tuple( std::move( out1 ), std::move( out2 ), std::move( trivial_selection ) );
      }
    };
    DECLARE_COMPONENT( Cook )
  } // namespace Examples
} // namespace LHCb
