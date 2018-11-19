/*
File heavily inspired from https://github.com/therocode/meta_enum
Thus I repeat here the original license, as requested :

MIT License

Copyright (c) 2018 Tobias Widlund

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#pragma once
#include <array>
#include <tuple>
#include <string_view>
#include <optional>

namespace meta_enum_internal {
  
  template <typename EnumType>
  struct MetaEnumMember {
    EnumType value = {};
    std::string_view name;
  };

  template <typename EnumType, typename UnderlyingTypeIn, size_t size>
  struct MetaEnum {
    using UnderlyingType = UnderlyingTypeIn;
    std::array<MetaEnumMember<EnumType>, size> members = {};
    std::string_view defaultString;
    EnumType defaultValue{0};
  };

  constexpr bool isNested (size_t brackets, bool quote) {
    return brackets != 0 || quote;
  }

  constexpr size_t nextEnumCommaOrEnd(size_t start, std::string_view enumString) {
    size_t brackets = 0; //()[]{}
    bool quote = false; //""
    char lastChar = '\0';
    char nextChar = '\0';

    auto feedCounters = [&brackets, &quote, &lastChar, &nextChar] (char c) {
      if(quote) {
        if(lastChar != '\\' && c == '"') //ignore " if they are backslashed
          quote = false;
        return;
      }
      switch(c) {
      case '"':
        if(lastChar != '\\') //ignore " if they are backslashed
          quote = true;
        break;
      case '(':
      case '<':
        if(lastChar == '<' || nextChar == '<')
          break;
      case '{':
        ++brackets;
        break;
      case ')':
      case '>':
        if(lastChar == '>' || nextChar == '>')
          break;
      case '}':
        --brackets;
        break;
      default:
        break;
      }
    };

    size_t current = start;
    for(; current < enumString.size() && (isNested(brackets, quote) || (enumString[current] != ',')); ++current) {
      feedCounters(enumString[current]);
      lastChar = enumString[current];
      nextChar = current + 2 < enumString.size() ? enumString[current + 2] : '\0';
    }
    return current;
  }

  constexpr bool isAllowedIdentifierChar(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           c == '_';
  }

  constexpr std::string_view parseEnumMemberName(std::string_view memberString) {
    size_t nameStart = 0;
    while(!isAllowedIdentifierChar(memberString[nameStart])) {
      ++nameStart;
    }
    size_t nameSize = 0;
    while(isAllowedIdentifierChar(memberString[nameStart + nameSize])) {
      ++nameSize;
    }
    return std::string_view(memberString.data() + nameStart, nameSize);
  }

  template<typename EnumType, size_t size>
    using Values = std::array<EnumType, size>;
  template<typename EnumType, size_t size>
    using ValuesWithDefault = std::tuple<EnumType, Values<EnumType, size>>;

  template <typename EnumType, typename UnderlyingType, size_t size>
    constexpr MetaEnum<EnumType, UnderlyingType, size>
    parseMetaEnum(std::string_view in,
                  const ValuesWithDefault<EnumType, size>& values) {
    MetaEnum<EnumType, UnderlyingType, size> result;
    std::array<std::string_view, size> memberStrings;
    size_t amountFilled = 0;
    size_t currentStringStart = 0;
    while(amountFilled < size) {
      size_t currentStringEnd = nextEnumCommaOrEnd(currentStringStart + 1, in);
      size_t currentStringSize = currentStringEnd - currentStringStart;
      if(currentStringStart != 0) {
        ++currentStringStart;
        --currentStringSize;
      }
      memberStrings[amountFilled] = std::string_view(in.data() + currentStringStart, currentStringSize);
      ++amountFilled;
      currentStringStart = currentStringEnd;
    }
    for(size_t i = 0; i < memberStrings.size(); ++i) {
      result.members[i].name = parseEnumMemberName(memberStrings[i]);
      result.members[i].value = std::get<1>(values)[i];
      if (std::get<0>(values) == std::get<1>(values)[i]) {
        result.defaultString = result.members[i].name;
        result.defaultValue = std::get<0>(values);
      }
    }
    return result;
  }

  template <typename EnumUnderlyingType>
    struct IntWrapper {
      constexpr IntWrapper() = default;
      constexpr IntWrapper(EnumUnderlyingType in): value(in), empty(false) {}
      constexpr IntWrapper operator=(EnumUnderlyingType in) {
        value = in;
        empty = false;
        return *this;
      }
      EnumUnderlyingType value{0};
      bool empty{true};
    };

  template <typename EnumType, typename EnumUnderlyingType, size_t size>
    constexpr ValuesWithDefault<EnumType, size>
    resolveEnumValuesArray
    (const EnumType defaultValue,
     const std::initializer_list<IntWrapper<EnumUnderlyingType>>& in) {
    Values<EnumType, size> result{};
    EnumUnderlyingType nextValue = 0;
    for(size_t i = 0; i < size; ++i) {
      auto wrapper = *(in.begin() + i);
      EnumUnderlyingType newValue = wrapper.empty ? nextValue : wrapper.value;
      nextValue = newValue + 1;
      result[i] = static_cast<EnumType>(newValue);
    }
    return {defaultValue, result};
  }
}

/**
 * This macro allows to declare enums that can be automatically
 * pretty printed and parsed from strings.
 * Practically, you get a toString and parse method as well as the
 * standard operator<<.
 * The arguments of the macro are
 *   - name of the Enum to create
 *   - type to be used to store in enum, typically int
 *   - name of default entry, used when e.g. the parsing fails
 *   - ... list of entries, in the form "Name" or "Name=value"
 *     to force a given value
 */
#define meta_enum_class_with_unknown(Type, UnderlyingType, Default, ...)\
  enum class Type : UnderlyingType { __VA_ARGS__};\
  constexpr static auto Type##_internal_size = [] () constexpr {\
    using IntWrapperType = meta_enum_internal::IntWrapper<UnderlyingType>;\
    IntWrapperType __VA_ARGS__;\
    return std::initializer_list<IntWrapperType>{__VA_ARGS__}.size();\
  };\
  constexpr static auto Type##_meta = meta_enum_internal::parseMetaEnum<Type, UnderlyingType, Type##_internal_size()>(#__VA_ARGS__, []() { \
    using IntWrapperType = meta_enum_internal::IntWrapper<UnderlyingType>;\
    IntWrapperType __VA_ARGS__;\
    return meta_enum_internal::resolveEnumValuesArray<Type, UnderlyingType, Type##_internal_size()>(Type::Default, {__VA_ARGS__}); \
  }());\
  inline std::string toString(const Type e) {\
    for(const auto& member : Type##_meta.members) {\
      if(member.value == e) return std::string{member.name};\
    }\
    return std::string{Type##_meta.defaultString};\
  }\
  inline std::ostream& operator<< (std::ostream& s, const Type e) {\
    return s << toString(e);\
  }\
  inline StatusCode parse(Type& e, std::string_view name) {\
    for(const auto& member : Type##_meta.members) {\
      if (member.name == name) {\
        e = member.value;\
        return StatusCode::SUCCESS;\
      }\
    }\
    e = Type##_meta.defaultValue;\
    return StatusCode::FAILURE;\
  }

/**
 * This macro allows to declare enums that can be automatically
 * pretty printed and parsed from strings.
 * This is an improvement on meta_enum_class_with_unknown where
 * the Default is automatically "Unknown". This implies that the
 * enum declared must have an "Unknown" entry or this will trigger
 * a compiler error.
 */
#define meta_enum_class(Type, UnderlyingType, ...) meta_enum_class_with_unknown(Type, UnderlyingType, Unknown, __VA_ARGS__)
