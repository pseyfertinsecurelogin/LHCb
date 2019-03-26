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

#include "SOAContainer/SOAContainer.h"
#include "SOAContainer/SOAField.h"
#include "SOAContainer/SOASkin.h"
#include "SOAExtensions/ZipContainer.h"
#include <vector>

namespace Examples {

  struct Apple {
    float m_weight; // in SI units, i.e. kg

    void  setWeight( float w ) { m_weight = w; }
    float weight() const { return m_weight; }

    // comparison operators are hacked to identify the same apple
    bool operator==( const Apple& other ) const { return &other == this; }
    bool operator!=( const Apple& other ) const { return &other != this; }
  };

  struct NutritionalContent {
    float m_calories; // not in SI units, because nobody uses them.
    float calories() const { return m_calories; }
    void  setCalories( float c ) { m_calories = c; }
    float m_vitaminC; // mol??? yeah, mol seems right as unit
    bool  operator==( const NutritionalContent& other ) const { return &other == this; }
    bool  operator!=( const NutritionalContent& other ) const { return &other != this; }
  };

  struct MarketValue {
    float m_looks; /// how good it looks to the consumer on a scale from 0 to 10.
    float looks() { return m_looks; }
    void  setLooks( float l ) { m_looks = l; }
    bool  operator==( const MarketValue& other ) const { return &other == this; }
    bool  operator!=( const MarketValue& other ) const { return &other != this; }
  };

  // SOA::Field s are the harder part to understand about the SOAContainer
  // framework.  If you're familiar with "tagged types", it's roughly that.  If
  // you're not familiar with tagged types, then I'm afraid the necessity for
  // fields is not obvious from this example. Probably you can go ahead and
  // ignore these three lines and remember that "fields go into a skin", and
  // "types do not go into a skin".
  SOAFIELD_TRIVIAL( AppleField, apple, Apple );
  SOAFIELD_TRIVIAL( NutritionalContentField, nutritionalContent, NutritionalContent );
  SOAFIELD_TRIVIAL( MarketValueField, marketValue, MarketValue );

  // SOA::Skins are basically the types that govern what zipping does. (which
  // "fields" zipped together result in which APIs).
  // These macros work as 'SOASKIN_TRIVIAL( <skin that gets defined> , <fields that get zipped> ... );
  //
  // TRIVIAL skin definitions only provide the minimal automatically generatable API.
  SOASKIN_TRIVIAL( AppleSkin, AppleField );
  SOASKIN_TRIVIAL( NutritionalContentSkin, NutritionalContentField );

  // I define these skins now a bit more verbose to demonstrate how "skin methods" can be used.
  // Now we can use calories and weight around or store the ratio, but keep the calories_per_100g() method in place for
  // user code.
  SOASKIN( StoreSkin, AppleField, MarketValueField ) {
    SOASKIN_INHERIT_DEFAULT_METHODS( StoreSkin );
    float weight() { return this->apple().weight(); }
  };
  SOASKIN( Harvester_OutputSkin, AppleField, NutritionalContentField ) {
    SOASKIN_INHERIT_DEFAULT_METHODS( Harvester_OutputSkin );
    float weight() { return this->apple().weight(); }
    float calories_per_100g() { return 0.1f * this->nutritionalContent().calories() / this->apple().weight(); }
  };
  SOASKIN( FullAppleSkin, AppleField, NutritionalContentField, MarketValueField ) {
    SOASKIN_INHERIT_DEFAULT_METHODS( FullAppleSkin );
    float calories_per_100g() { return 0.1f * this->nutritionalContent().calories() / this->apple().weight(); }
  };

  SOASKIN( MarketValueSkin, MarketValueField ) {
    SOASKIN_INHERIT_DEFAULT_METHODS( MarketValueSkin );
    MarketValueSkin( float x ) {
      if ( x < 0.f )
        this->marketValue().setLooks( 0.f );
      else if ( x < 10.f )
        this->marketValue().setLooks( 10.f );
      else
        this->marketValue().setLooks( x );
    }
  };

  struct FruitSalad {
    std::vector<size_t>        m_appleIndices; /// references to individual objects are missing in SOAExtensions
    const std::vector<size_t>& appleIndices() const { return m_appleIndices; }
    void                       setAppleIndices( std::vector<size_t> ai ) { m_appleIndices = ai; }
  };

  struct Dish {
    float m_totalWeight;
    float m_totalCalories;
  };

  SOAFIELD_TRIVIAL( FruitSaladField, fruitSalad, FruitSalad );
  SOAFIELD_TRIVIAL( DishField, dish, Dish );

  SOASKIN_TRIVIAL( FruitSaladSkin, FruitSaladField );
  SOASKIN_TRIVIAL( DishSkin, DishField );

  SOASKIN_TRIVIAL( FullPlateSkin, FruitSaladField, DishField );

} // namespace Examples
