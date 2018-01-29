#pragma once

namespace LHCb {
  namespace Tag {

    namespace State {
      struct AssumeUnordered_tag { };
      struct AssumeSorted_tag : AssumeUnordered_tag { };

      constexpr auto AssumeUnordered = AssumeUnordered_tag{};
      constexpr auto AssumeSorted    = AssumeSorted_tag{};
    }

  }
}
