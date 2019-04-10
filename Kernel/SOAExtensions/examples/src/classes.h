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

/** @file SOAExtensions/examples/src/classes.h
 *
 * @brief data types for the apple harvesting example
 *
 * General walkthrough for the example is in @see SOAExtensions/examples/src/apples.cpp
 */
namespace Examples {

  /** @class Apple classes.h
   *
   * @brief some, but not all information about an apple.
   *
   * Data types in an SOA container can be any C++ type. For lack of
   * creativity/to keep the example simple, the class here contains just a
   * float. For usage in `find` (to find a specifc apple in a selection / view)
   * the apple has an unconventional comparison operator. This is to avoid the
   * trouble of floating point comparisons within the example.
   */
  struct Apple {
    float m_weight; ///< weight of an apple in SI units, i.e. kg

    /// setter for apple weight
    void setWeight( float w ) { m_weight = w; }
    /// getter for apple weight
    float weight() const { return m_weight; }

    /// apple equality operator (assume no two apples in nature are equal)
    bool operator==( const Apple& other ) const { return &other == this; }
    /// apple inequality operator (assume no two apples in nature are equal)
    bool operator!=( const Apple& other ) const { return &other != this; }
  };

  /** @class NutritionalContent class.h
   *
   * @brief some other information about an apple (@see Apple class)
   *
   * For within the example, the 'NutritionalContent' information is produced
   * by the same algorithm that produces the 'Apple'.
   */
  struct NutritionalContent {
    float m_calories; ///< caloric energy of an apple, in calories because nobody uses the SI unit in practice.

    /// getter for calories
    float calories() const { return m_calories; }
    /// setter for calories
    void setCalories( float c ) { m_calories = c; }

    float m_vitaminC; ///< vitamin C content of the apple, in mol.

    /// equality operator (same as for Examples::Apple, not meant to be realistic c++ but convenience in example)
    bool operator==( const NutritionalContent& other ) const { return &other == this; }
    /// inequality operator (same as for Examples::Apple, not meant to be realistic c++ but convenience in example)
    bool operator!=( const NutritionalContent& other ) const { return &other != this; }
  };

  /** @class MarketValue class.h
   *
   * @brief some more information about an apple (@see Apple class)
   *
   * In contrast to the 'NutritionalContent' information, the 'MarketValue' is
   * produced by a different algorithm than the 'Apple'. (This is not visible
   * from the struct definition, but only turns out this way in the data flow).
   * This may be used in reality to compute genuine new information, but also
   * to store computationally expensive data that could be derived from
   * existing data.
   */
  struct MarketValue {
    /// how good it looks to the consumer on a scale from 0 to 10.
    float m_looks;
    /// getter for the apple's looks
    float looks() { return m_looks; }
    /// setter for the apple's looks
    void setLooks( float l ) { m_looks = l; }
    /// equality operator (same as for Examples::Apple, not meant to be realistic c++ but convenience in example)
    bool operator==( const MarketValue& other ) const { return &other == this; }
    /// inequality operator (same as for Examples::Apple, not meant to be realistic c++ but convenience in example)
    bool operator!=( const MarketValue& other ) const { return &other != this; }
  };

  // SOA::Field s are the harder part to understand about the SOAContainer
  // framework.
  //
  // If you're familiar with "tagged types", it's roughly that.  If
  // you're not familiar with tagged types, then I'm afraid the necessity for
  // fields is not obvious from this example. Probably you can go ahead and
  // ignore these three lines and remember that "fields go into a skin", and
  // "types do not go into a skin".
  //
  // See also the documentation in SOAField.h

  /**
   * @brief Makes the 'Apple' C++ type available to the SOA framework
   *
   * @param AppleField name of the hereby defined type AppleField
   * @param apple      name of the hereby defined accessor function apple()
   * @param Apple      regular C++ type that is made avalable to SOA
   */
  SOAFIELD_TRIVIAL( AppleField, apple, Apple );

  /**
   * @brief  Makes the 'NutritionalContent' C++ type available to the SOA framework
   *
   * @param NutritionalContentField name of the hereby defined type NutritionalContentField
   * @param nutritionalContent      name of the hereby defined accessor function nutritionalContent()
   * @param NutritionalContent      regular C++ type that is made avalable to SOA
   */
  SOAFIELD_TRIVIAL( NutritionalContentField, nutritionalContent, NutritionalContent );

  /**
   * @brief   Makes the 'MarketValueField' C++ type available to the SOA framework
   *
   * @param MarketValueField name of the hereby defined type MarketValueField
   * @param marketValue      name of the hereby defined accessor function marketValue()
   * @param MarketValue      regular C++ type that is made avalable to SOA
   */
  SOAFIELD_TRIVIAL( MarketValueField, marketValue, MarketValue );

  // SOA::Skins are the types exposed to the user of a zip.
  //
  // The user facing API of "objects" in SOA Containers are defined here.
  // Trivial skins do not have any other API than accessors and constructors.
  // These macros work as 'SOASKIN_TRIVIAL( <skin that gets defined> , <fields that get zipped> ... );
  //
  // See also the documentation of SOASKIN_TRIVIAL in SOASkin.h

  /**
   * @brief API of "objects" (technically 'proxies') in SOA Containers of 'Apples'.
   *
   * @param AppleSkin  name of the hereby defined type
   * @param AppleField field that is available in this skin
   */
  SOASKIN_TRIVIAL( AppleSkin, AppleField );
  /**
   * @brief  API of "objects" (technically 'proxies') in SOA Containers of 'NutritionalContentSkin'.
   *
   * @param NutritionalContentSkin  name of the hereby defined type
   * @param NutritionalContentField field that is available in this skin
   */
  SOASKIN_TRIVIAL( NutritionalContentSkin, NutritionalContentField );

  /**
   * @brief API for proxies in SOA Containers with 'AppleField' and 'MarketValueField' (zipped, or directly created)
   *
   * The skin here offers the possiblity to add methods to the proxy. The
   * weight() method here allows the core developers to move the actual weight
   * data from one field to another, store in a different unit, or generally
   * change the representation of the information.
   *
   * In this simplified example however the method only shortens
   * proxy.apple().weight() to proxy.apple()
   *
   * @param StoreSkin        name of the hereby defined type
   * @param AppleField       field that is available in this skin
   * @param MarketValueField field that is available in this skin
   */
  SOASKIN( StoreSkin, AppleField, MarketValueField ) {
    /**
     * @brief Create all default methods (accessors and constructors)
     */
    SOASKIN_INHERIT_DEFAULT_METHODS( StoreSkin );
    /**
     * @return returns the weight from the zipped 'Apple' component
     */
    float weight() { return this->apple().weight(); }
  };

  /**
   * @brief API for proxies in SOA Containers with 'AppleField' and 'NutritionalContentField'
   *
   * The skin here offers the possiblity to add methods to the proxy. The
   * weight() method here allows the core developers to move the actual weight
   * data from one field to another, store in a different unit, or generally
   * change the representation of the information.
   *
   * The calories_per_100g() method can be kept in place even if 'm_calories'
   * or 'm_weight' are moved around between the zip components, or if the core
   * developers choose to store directly the calories per 100g. Thus backend
   * changes are decoupled from the API.
   *
   * @param Harvester_OutputSkin    name of the hereby defined type
   * @param AppleField              field that is available in this skin
   * @param NutritionalContentField field that is available in this skin
   */
  SOASKIN( Harvester_OutputSkin, AppleField, NutritionalContentField ) {
    /**
     * @brief Create all default methods (accessors and constructors)
     */
    SOASKIN_INHERIT_DEFAULT_METHODS( Harvester_OutputSkin );
    /**
     * @return returns the weight from the zipped 'Apple' component
     */
    float weight() { return this->apple().weight(); }
    /**
     * @return returns the calories per 100g weight to combine data from two fields.
     */
    float calories_per_100g() { return 0.1f * this->nutritionalContent().calories() / this->apple().weight(); }
  };

  /**
   * @brief API for proxies in SOA Containers with 'AppleField', 'NutritionalContentField', and 'MarketValueField'
   *
   * The skin here offers the possiblity to add methods to the proxy. The
   * weight() method here allows the core developers to move the actual weight
   * data from one field to another, store in a different unit, or generally
   * change the representation of the information.
   *
   * The calories_per_100g() method can be kept in place even if 'm_calories'
   * or 'm_weight' are moved around between the zip components, or if the core
   * developers choose to store directly the calories per 100g. Thus backend
   * changes are decoupled from the API.
   *
   * @param FullAppleSkin            name of the hereby defined type
   * @param AppleField               field that is available in this skin
   * @param NutritionalContentField  field that is available in this skin
   * @param MarketValueField         field that is available in this skin
   */
  SOASKIN( FullAppleSkin, AppleField, NutritionalContentField, MarketValueField ) {
    /**
     * @brief Create all default methods (accessors and constructors)
     */
    SOASKIN_INHERIT_DEFAULT_METHODS( FullAppleSkin );
    /**
     * @return returns the calories per 100g weight to combine data from two fields.
     */
    float calories_per_100g() { return 0.1f * this->nutritionalContent().calories() / this->apple().weight(); }
  };

  /**
   * @brief API for proxies in SOA Containers with 'NutritionalContentField', and 'MarketValueField'
   *
   * @param MarketValueSkin          name of the hereby defined type
   * @param MarketValueField         field that is available in this skin
   */
  SOASKIN( MarketValueSkin, MarketValueField ) {
    /**
     * @brief Create all default methods (accessors and constructors)
     */
    SOASKIN_INHERIT_DEFAULT_METHODS( MarketValueSkin );
    /**
     * @brief Custom constructor
     *
     * @param x the desired market value (capped between 0 and 10)
     */
    MarketValueSkin( float x ) {
      if ( x < 0.f )
        this->marketValue().setLooks( 0.f );
      else if ( x < 10.f )
        this->marketValue().setLooks( 10.f );
      else
        this->marketValue().setLooks( x );
    }
  };

  /** @class FruitSalad classes.h
   *
   * @brief  Yet another class for the example (multiple apples go into one fruit salad)
   */
  struct FruitSalad {
    std::vector<size_t> m_appleIndices; ///< references to individual objects are missing in SOAExtensions
    /// getter of m_appleIndices
    const std::vector<size_t>& appleIndices() const { return m_appleIndices; }
    /// setter of m_appleIndices
    void setAppleIndices( std::vector<size_t> ai ) { m_appleIndices = ai; }
  };

  /** @class Dish classes.h
   *
   * @brief A zip'able component for the FruitSalad (just to have something to zip)
   */
  struct Dish {
    float m_totalWeight;   ///< the total weight of all apples in a fruit salad (so they don't have to be summed)
    float m_totalCalories; ///< some information that is printed in the menu at the restaurant
  };

  /** @brief field definition for the FruitSalad
   *
   * Nothing meaningful to be documented here beyond copy and paste from other field in classes.h
   */
  SOAFIELD_TRIVIAL( FruitSaladField, fruitSalad, FruitSalad );

  /** @brief field definition for the Dish
   *
   * Nothing meaningful to be documented here beyond copy and paste from other field in classes.h
   */
  SOAFIELD_TRIVIAL( DishField, dish, Dish );

  /** @brief skin definition for the FruitSalad
   *
   * Nothing meaningful to be documented here beyond copy and paste from other skin in classes.h
   */
  SOASKIN_TRIVIAL( FruitSaladSkin, FruitSaladField );
  /** @brief skin definition for the Dish
   *
   * Nothing meaningful to be documented here beyond copy and paste from other skin in classes.h
   */
  SOASKIN_TRIVIAL( DishSkin, DishField );

  /** @brief skin definition for the FruitSalad and Dish
   *
   * Nothing meaningful to be documented here beyond copy and paste from other skin in classes.h
   */
  SOASKIN_TRIVIAL( FullPlateSkin, FruitSaladField, DishField );

} // namespace Examples
