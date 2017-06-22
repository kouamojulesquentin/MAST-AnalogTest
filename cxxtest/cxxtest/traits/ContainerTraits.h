//===========================================================================
//                           ContainerTraits.h
//===========================================================================
//! @file Qt_Traits_Commons.h
//!
//! Defines ContainerTraits (helper for displaying container contents)
//!
//===========================================================================


#ifndef CONTAINERTRAITS_H__39D99486_3ADE_490D_E2B5_C3E87365FF77__INCLUDED_
  #define CONTAINERTRAITS_H__39D99486_3ADE_490D_E2B5_C3E87365FF77__INCLUDED_

#include <string>
#include <sstream>
#include <iomanip>
#include <tuple>
#include <memory>
#include <cxxtest/ValueTraits.h>
#include <cxxtest/traits/EnhancedValueTraits.h>

namespace CxxTest
{
#ifndef CONTAINERS_TRAITS_MAX_ITEMS
  #define CONTAINERS_TRAITS_MAX_ITEMS 10U
#endif

#ifndef CONTAINERS_TRAITS_LAST_ITEMS_COUNT
  #define CONTAINERS_TRAITS_LAST_ITEMS_COUNT 3U
#endif

// ---------------- Types aliases
//
using Size_t_RestorationLambda = std::function<void(size_t*)>;
using Size_t_Restorer          = std::unique_ptr<size_t, Size_t_RestorationLambda> ;
template<typename TIndex, typename TItem>
using TLineStreamer = std::function<void(std::ostringstream& os, TIndex index, const TItem& item)>;

// ---------------- Inner class helpers
//
//! Provides streaming of an item for which their is no "enhanced" ValueTrait
//!
//! @note indent is not communicated to item ValueTrait
//!
template <bool EnhancedValueTraits> class ItemStreamer
{
  public:
  template<typename TItem, typename TIndex>
  static void Stream(std::ostringstream& os, int digitCount, std::string indent, TIndex index, const TItem& item)
  {
    auto itemTrait    = traits(item);
    auto itemAsString = itemTrait.asString();

    os << indent << "[" << std::setw(digitCount) << index << "] " << itemAsString << ",\n";
  }
};


//! Provides streaming of an item for which their is an "enhanced" ValueTrait
//!
//! @note indent is communicated to item ValueTrait constructor along with the
//!       fact that it is emmbedded (in some kind of container)
//!
template<> class ItemStreamer<true>
{
  public:
  template<typename TItem, typename TIndex>
  static void Stream(std::ostringstream& os, int digitCount, std::string indent, TIndex index, const TItem& item)
  {
    std::ostringstream indexStream;
    indexStream << indent << "[" << std::setw(digitCount) << index << "] ";
    auto indexText = indexStream.str();
    os << indexText;
    indent = std::string(indexText.length(), ' ');

    auto itemTrait    = ValueTraits<TItem>(item, indent, true);
    auto itemAsString = itemTrait.asString();

    os << itemAsString << ",\n";
  }
};


//! Provides a ways to customize, for containers, the number of displayed items.
//! @note - When there are few items, there is no problem displaying them all, but when
//!         there are plenty of them it is often useless to display them all.
//!         But sometimes one can want to display more than the default limit.
//!       - The default limit is define by CONTAINERS_TRAITS_MAX_ITEMS that can be
//!         overidden by compile defines
//!       - When some items are not displayed, first and last items are displayed
//!       - Last items count is, by default defined by CONTAINERS_TRAITS_LAST_ITEMS
//!
template<typename TContainer>
class ContainerTraits : public EnhancedValueTraits
{
  template <typename> friend class ValueTraits;

  // ---------------- Public Methods
  //
  public:

  //! Returns current number of last items displayed (when there are too much to display)
  //!
  static size_t LastDisplayedCount() { return sm_lastDisplayedCount; };

  //! Returns current maximum displayed items
  //!
  static size_t MaxDisplayedItems() { return sm_maxDisplayedItems; };

  //! Changes the maximum number of of last items displayed (when there are too much to display)
  //!
  //! @param newLastDisplayedCount New maximum displayed items
  //!
  //!
  static size_t SetLastDisplayedCount(size_t newLastDisplayedCount)
  {
    auto previousValue    = sm_lastDisplayedCount;
    sm_lastDisplayedCount = newLastDisplayedCount;

    return previousValue;
  }

  //! Changes the maximum number of of last items displayed (when there are too much to display)
  //!
  //! @param newLastDisplayedCount New maximum displayed items
  //!
  //! @return An object that restores previous value when destructed
  static Size_t_Restorer SetLastDisplayedCount_WithAutoRestore(size_t newLastDisplayedCount)
  {
    auto previousValue    = sm_lastDisplayedCount;
    sm_lastDisplayedCount = newLastDisplayedCount;

    auto restorer = [previousValue](size_t* ptr)
    {
      *ptr = previousValue;
    };

    return Size_t_Restorer(&sm_lastDisplayedCount, restorer);
  }

  //! Changes the maximum number of container items displayed
  //! (beyond that value, first items and last three items will be displayed, totalling this max value)
  static size_t SetMaxDisplayedItems(size_t newMaxDisplayedItems)
  {
    auto previousValue   = sm_maxDisplayedItems;
    sm_maxDisplayedItems = newMaxDisplayedItems;
    return previousValue;
  }

  //! Changes the maximum number of container items displayed
  //! (beyond that value, first items and last CONTAINERS_TRAITS_LAST_ITEMS items will be displayed,
  //! totalling this max value)
  //!
  //! @param newMaxDisplayedItems New maximum displayed items
  //!
  //! @return An object that restore previous value when destructed
  //!
  static Size_t_Restorer SetMaxDisplayedItems_WithAutoRestore(size_t newMaxDisplayedItems)
  {
    auto previousValue = sm_maxDisplayedItems;
    sm_maxDisplayedItems = newMaxDisplayedItems;
    auto restorer = [previousValue](size_t* ptr)
    {
      *ptr = previousValue;
    };

    return Size_t_Restorer(&sm_maxDisplayedItems, restorer);
  }



  // ---------------- Protected Methods
  //

  //! Returns whether some container items are not displayed and, if yes, the number displayed
  //! from the beginning and the end of the container.
  //!
  static std::tuple<bool, size_t, size_t>  DisplayCounts(size_t containerCount)
  {
    auto limitDisplay        = containerCount > sm_maxDisplayedItems;
    auto lastDisplayedCount  = std::min(sm_lastDisplayedCount, sm_maxDisplayedItems);
    auto firstDisplayedCount = limitDisplay ? sm_maxDisplayedItems - lastDisplayedCount : containerCount;

    return std::make_tuple(limitDisplay, firstDisplayedCount, lastDisplayedCount);
  }

  //! Streams the "body" of a container into an output string
  //!
  template<typename TItem, typename TIndex>
  static void StreamBody(const TContainer& container, TIndex count, std::ostringstream& os, const std::string& indent, bool embeddedInContainer)
  {
    auto indentLevel_2       = indent + std::string("  ");
    auto digitCount          = std::ceil(std::log10(count));
    auto limitDisplay        = false;
    auto firstDisplayedCount = size_t(0);
    auto lastDisplayedCount  = size_t(0);

    std::tie(limitDisplay, firstDisplayedCount, lastDisplayedCount) = DisplayCounts(count);

    using TStreamer = ItemStreamer<std::is_base_of<EnhancedValueTraits, ValueTraits<TItem>>::value>;

    os << indent << "{\n";

    // ---------------- Display first items (or all items if there are few of them)
    //
    auto index = TIndex(0);

    if (!limitDisplay || (firstDisplayedCount != 0))
    {
      for (auto const& item : container)
      {
        TStreamer::Stream(os, digitCount, indentLevel_2, index, item);
        ++index;

        if (limitDisplay && (index == static_cast<TIndex>(firstDisplayedCount)))
        {
          break;
        }
      }
    }

    // ---------------- Display last items of container (if not all displayed)
    //
    if (limitDisplay)
    {
      os << indentLevel_2 << "...\n";
      if (lastDisplayedCount != 0)
      {
        index = count - lastDisplayedCount;
        while (index < count)
        {
          auto const& item = container[index];
          TStreamer::Stream(os, digitCount, indentLevel_2, index, item);
          ++index;
        }
      }
    }

    os << indent << "}";
    if (!embeddedInContainer)
    {
      os << "\n";
    }
  }


  // ---------------- Private  Fields
  //
  private:
  static size_t sm_maxDisplayedItems;
  static size_t sm_lastDisplayedCount;
};

template<typename TContainer>
size_t ContainerTraits<TContainer>::sm_maxDisplayedItems  = CONTAINERS_TRAITS_MAX_ITEMS;

template<typename TContainer>
size_t ContainerTraits<TContainer>::sm_lastDisplayedCount = CONTAINERS_TRAITS_LAST_ITEMS_COUNT;

} // End of namespace CxxTest

#endif  // not defined CONTAINERTRAITS_H__39D99486_3ADE_490D_E2B5_C3E87365FF77__INCLUDED_
//===========================================================================
// End of ContainerTraits.h
//===========================================================================


