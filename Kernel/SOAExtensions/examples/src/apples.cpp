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

/** @file
 * A minimal sequence of functional algorithms passing ZipContainers and ExportedSelections around
 *
 * The example is around harvesting of fruit. Apples are first harvested by a
 * `Harvester`, this puts the start of a container family to the TES.
 *
 * The apples are then sorted by a `Sorter` (imagine a person sorting out which
 * apples we want to process further). This produces a non-trivial
 * 'ExportedSelection' object, which will then be used by algorithms later in
 * the sequence to process or not process certain apples.
 *
 * Additional information for the apples are then computed by a `Rater`.
 * Compared to the "old framework", the rater does neither alter the apples on
 * TES, nor copy the apples and update some data members of them. The ratings
 * of the apples are put on TES and can then be read by later algorithms in the
 * sequence by means of a zip.
 *
 * A cook in the end is inspired by a vertexer: The cook produces fruit salads.
 * Each salad consisting of several apples (in the current implementation
 * exactly two, and each apple can be used in multiple salads). The Cook
 * produces the start of a new container family.
 */

namespace LHCb {
  namespace Examples {
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

    /** @class Harvester
     *
     * @brief an algorithm that writes "apples" to TES (seeded by an integer from GaudiExamples)
     *
     * @see Kernel/SOAExtensions/examples/src/apples.cpp for an overview of the example
     */
    struct Harvester final : Gaudi::Functional::MultiTransformer<HarvesterOutput( const int& )> {
      /// constructor (standard GaudiFunctional)
      Harvester( const std::string& name, ISvcLocator* svcLoc )
          : MultiTransformer( name, svcLoc, {KeyValue{"Seed", "/Event/MyInt"}},
                              {KeyValue{"Apple_Output", "/Event/Apples"},
                               KeyValue{"Nutrient_Output", "/Event/Nutrients"},
                               KeyValue{"Selection_Output", "/Event/HarvesterSelection"}} ) {}

      // counters mainly to see something in the logfiles
      mutable Gaudi::Accumulators::AveragingCounter<int> m_harvested{
          this, "picked heavy apples"}; ///< a counter that counts produced apples
      mutable Gaudi::Accumulators::AveragingCounter<float> m_weights{this, "apple weights"}; ///< a counter that
                                                                                             ///< averages apple weights

      /**
       * @brief produce apples
       *
       * @param seed an integer (not really necessary for example, but assume most people want a transformer example)
       *
       * @return two zip'able containers (apples and their nutrients) and a selection that selects all apples
       *
       * @see Kernel/SOAExtensions/examples/src/classes.h for the example specific data types (apples and nutrients)
       */
      HarvesterOutput operator()( const int& seed ) const override {

        // Technically we're not zipping anything within the Harvester, but to
        // keep only one container API for producing and transforming, we're
        // using them anyway.
        Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>> part1;

        // zip-compatibility with the previous container is "declared" through
        // the constructor argument (the `part1.zipIdentifier()`).
        // Leaving that part out and writing only `part2;` would work within the Harvester, but crash later.
        Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>> part2(
            part1.zipIdentifier() ); // assign family number at construction!

        // now create some apples with rather meaningless numbers
        for ( int i = 0; i < 42; ++i ) {
          part1.push_back( ::Examples::Apple{0.2f + 0.001f * ( seed + i )} ); // an apple weighs 200g plus a bit more
          part2.push_back( ::Examples::NutritionalContent{20.f, 500} );
        }

        bool do_monitoring = true;
        if ( do_monitoring ) {
          // Instead of using part1 and part2 separately, one can also zip them
          // directly: Sadly push_back/emplace_back cannot be used on zips (so
          // can't be used above for creating the apples.  I'd prefer `auto
          // apples = zip(part1,part2); apples.push_back(....); It would work
          // if we didn't intend to write the components separately to TES
          auto apples = Zipping::semantic_zip<::Examples::Harvester_OutputSkin>( part1, part2 );
          auto buf1   = m_harvested.buffer();
          auto buf2   = m_weights.buffer();

          // Loop over apples and do something.
          // Using STL algorithms is possible, too.
          for ( auto apple : apples ) {
            if ( apple.weight() > 0.210f ) buf1 += 1;
            buf2 += apple.weight();
          }
        }

        // There are multiple constructors for Selections. This one is the easiest select-everything.
        // It doesn't matter if part1, part2, or apples is used here - only the size is relevant.
        Zipping::ExportedSelection<> trivial_selection{part1, Zipping::details::alwaysTrue{}};

        // return the output to the main program
        HarvesterOutput retval =
            std::make_tuple( std::move( part1 ), std::move( part2 ), std::move( trivial_selection ) );
        return retval;
      }
    };
    DECLARE_COMPONENT( Harvester )

    /** @class Sorter
     *
     * @brief Refine a selection of apples
     *
     * The main part of the signature of a selection refining algorithm is
     *
     *  - In: some data containers and an Zipping::ExportedSelection
     *  - Out: an Zipping::ExportedSelection (refined version of the input ExportedSelection
     *
     * @see Kernel/SOAExtensions/examples/src/apples.cpp for an overview of the example
     */
    struct Sorter final
        : Gaudi::Functional::Transformer<Zipping::ExportedSelection<>(
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>&,
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>&,
              const Zipping::ExportedSelection<>& )> {
      /// apple selection criterion
      Gaudi::Property<float> m_minWeight{this, "minWeight", 0.225f, "minimum apple weight"};

      /// constructor (standard GaudiFunctional)
      Sorter( const std::string& name, ISvcLocator* svcLoc )
          : Transformer( name, svcLoc,
                         // clang-format off
                         {KeyValue{"Apple_Input", "/Event/Apples"},
                          KeyValue{"Nutrient_Input", "/Event/Nutrients"},
                          KeyValue{"Selection_Input", "/Event/HarvesterSelection"}
                          },
                         KeyValue{"Selection_Output", "/Event/SorterSelection"} ) {}
      // clang-format on

      /**
       * @brief read in apples and nutritional contents and select which apples shall be processed further
       *
       * @param p1 first zip component
       * @param p2 second zip component
       * @param e  preselection from the Harvester
       *
       * @return an ExportedSelection
       */
      Zipping::ExportedSelection<>
      operator()( const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>&              p1,
                  const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>& p2,
                  const Zipping::ExportedSelection<>& e ) const override {
        // p1, p2, e are variables only for the way from the framework to the creation of the SelectionView apple_input
        // afterwards they're not used anymore. apple_input is the interesting input object.
        // In other words the first two lines here are just boilerplate code
        auto                   z = Zipping::semantic_zip<::Examples::Harvester_OutputSkin>( p1, p2 );
        Zipping::SelectionView apple_input( &z, e );

        // selections can be refined with the `select` method that takes a
        // "callable" (presumably most will resort to lambdas like here):
        auto refined_exported_selection =
            apple_input.select( [&]( auto apple ) { return apple.weight() > m_minWeight; } );
        // the callable should return a bool (or something that can be
        // converted to bool) that is `true` if an object is selected, and
        // `false` for rejection.

        // counters for the efficiency get updated
        {
          size_t i      = 0;
          auto   buffer = m_npassed_cuts.buffer();
          for ( ; i < refined_exported_selection.size(); ++i ) buffer += true; // count the passes
          for ( ; i < apple_input.size(); ++i ) buffer += false;               // the rest is failed
        }

        return refined_exported_selection;
      }

      /// counter to monitor the Sorter's efficiency
      mutable Gaudi::Accumulators::BinomialCounter<> m_npassed_cuts{this, "apple acceptance efficiency"};
    };
    DECLARE_COMPONENT( Sorter )

    using RaterOutput = std::tuple<Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::MarketValueSkin>>,
                                   Zipping::ExportedSelection<>>;

    /** @class Rater
     *
     * @brief Produce some additional data for an existing container family
     *
     * @see Kernel/SOAExtensions/examples/src/apples.cpp for an overview of the example
     */
    struct Rater final
        : Gaudi::Functional::MultiTransformer<RaterOutput(
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>&,
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>&,
              const Zipping::ExportedSelection<>& )> {

      /// constructor (standard GaudiFunctional)
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

      /**
       * @brief  reads two zip'able containers and a selection, outputs an additional zip container and a selection
       *
       * @param p1 first input zip component
       * @param p2 second input zip component
       * @param e  selection of elements to be "upgraded"
       *
       * @return  additional data container (ZipContainer) and selection (ExportedSelection)
       */
      RaterOutput
      operator()( const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>&              p1,
                  const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>& p2,
                  const Zipping::ExportedSelection<>& e ) const override {
        // as in the Sorter, these two lines are boilerplate code
        auto                   all_input = Zipping::semantic_zip<::Examples::Harvester_OutputSkin>( p1, p2 );
        Zipping::SelectionView selected_input( &all_input, e );

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

        // create a container with the ratings
        Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::MarketValueSkin>> retval(
            all_input.zipIdentifier() );

        // since the outputs have to be zipped to the original container, a
        // push_back/emplace_back needs to be performed for *every* apple, not
        // only selected ones.
        for ( auto apple : all_input ) {
          // check if the apple got selected for further processing
          bool evaluate = ( selected_input.end() == std::find( selected_input.begin(), selected_input.end(), apple ) );

          if ( evaluate ) {
            // normally some expensive calculation would go here.
            // In this example the Rater just makes all selected apples top apples.
            retval.emplace_back( 10.f ); // top apple!
          } else {
            retval.emplace_back( 0.f ); // bad apple
          }
        }

        // No meaningful selection here, just preparing output
        Zipping::ExportedSelection<> trivial_selection( all_input, Zipping::details::alwaysTrue{} );
        return std::make_tuple( std::move( retval ), std::move( trivial_selection ) );
      }
    };
    DECLARE_COMPONENT( Rater )

    using KitchenOutput = std::tuple<Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::FruitSaladSkin>>,
                                     Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::DishSkin>>,
                                     Zipping::ExportedSelection<>>;
    /** @class Cook
     *
     * The 'Cook' algorithm creates new zip'able containers that aren't zip compatible with the input.
     *
     * In turn a new feature here is that the created objects refer to input object: Each salad has indices of the
     * apples it got created from.
     *
     * @brief Make fruit salads out of the apples
     *
     * @see Kernel/SOAExtensions/examples/src/apples.cpp for an overview of the example
     */
    struct Cook final
        : Gaudi::Functional::MultiTransformer<KitchenOutput(
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>&,
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>&,
              const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::MarketValueSkin>>&,
              const Zipping::ExportedSelection<>& )> {

      /// constructor (standard GaudiFunctional)
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

      /**
       * @brief transform apples to fruit salad
       *
       * @param in1 zip'able input container about apples
       * @param in2 zip'able input container about apples
       * @param in3 zip'able input container about apples
       * @param s   selected apples
       *
       * @return    zip'able containers for new container family "fruit salads"
       */
      KitchenOutput operator()(
          const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::AppleSkin>>& in1,
          [[maybe_unused]] const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::NutritionalContentSkin>>&
                                                                                                 in2,
          const Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::MarketValueSkin>>& in3,
          const Zipping::ExportedSelection<>&                                                    s ) const override {
        // boilerplate code for reading
        auto tmp   = Zipping::semantic_zip<::Examples::StoreSkin>( in1, in3 );
        auto input = Zipping::SelectionView( &tmp, s );

        // creation of output containers (just like in Harvester)
        Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::FruitSaladSkin>> out1;
        Zipping::ZipContainer<SOA::Container<std::vector, ::Examples::DishSkin>>       out2( out1.zipIdentifier() );

        // loop over all pairs of apples
        for ( size_t apple_index_1 = 0; apple_index_1 < input.size(); apple_index_1++ ) {
          for ( size_t apple_index_2 = apple_index_1 + 1; apple_index_2 < input.size(); apple_index_2++ ) {
            // get the actual apples
            auto apple1 = input[apple_index_1];
            auto apple2 = input[apple_index_2];

            // apply some cut on the combination of apples:
            // fruit salad must have a minimal weight
            if ( apple1.weight() + apple2.weight() > 0.205f ) {
              // If cuts are passed, create an output object by pushing into all output containers.
              // The first component keeps the indices of the inputs
              ::Examples::FruitSalad push_value1{std::vector<size_t>{apple_index_1, apple_index_2}};
              // Like before, mostly invented numbers.
              ::Examples::Dish push_value2{apple1.apple().weight() + apple2.apple().weight(),
                                           500.f}; // the label in the store just lies about the calories
              out1.push_back( push_value1 );
              out2.push_back( push_value2 );
            }
          }
        }

        // as before, a select-everyting selection goes along with every output
        Zipping::ExportedSelection<> trivial_selection( out1, Zipping::details::alwaysTrue{} );
        return std::make_tuple( std::move( out1 ), std::move( out2 ), std::move( trivial_selection ) );
      }
    };
    DECLARE_COMPONENT( Cook )
  } // namespace Examples
} // namespace LHCb
