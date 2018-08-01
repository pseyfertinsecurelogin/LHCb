#pragma once

namespace experimental
{
  namespace Tag
  {

    namespace State
    {
      struct AssumeUnfiltered_tag {
      };
      struct AssumeFiltered_tag : AssumeUnfiltered_tag {
      };

      constexpr auto AssumeUnordered = AssumeUnfiltered_tag{};
      constexpr auto AssumeFiltered  = AssumeFiltered_tag{};
    } // namespace State

  } // namespace Tag
} // namespace experimental

