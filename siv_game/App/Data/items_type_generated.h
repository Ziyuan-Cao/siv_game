// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ITEMSTYPE_FBS_BANDAI_SIV_GAME_BIN_INPUT_H_
#define FLATBUFFERS_GENERATED_ITEMSTYPE_FBS_BANDAI_SIV_GAME_BIN_INPUT_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 22 &&
              FLATBUFFERS_VERSION_MINOR == 12 &&
              FLATBUFFERS_VERSION_REVISION == 6,
             "Non-compatible flatbuffers version included");

namespace fbs_bandai_siv_game_bin_input {

struct item;
struct itemBuilder;

struct items_type;
struct items_typeBuilder;

struct item FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef itemBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ITEM_TYPE = 4,
    VT_NAME = 6,
    VT_ITEM_DATA = 8,
    VT_TIME = 10,
    VT_COLOR0 = 12,
    VT_COLOR1 = 14,
    VT_COLOR2 = 16,
    VT_SCALE = 18,
    VT_ITEM_WAS_PICKED_WORDS_TYPE = 20
  };
  const flatbuffers::String *item_type() const {
    return GetPointer<const flatbuffers::String *>(VT_ITEM_TYPE);
  }
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  float item_data() const {
    return GetField<float>(VT_ITEM_DATA, 10.0f);
  }
  int16_t time() const {
    return GetField<int16_t>(VT_TIME, 10);
  }
  float color0() const {
    return GetField<float>(VT_COLOR0, 0.9f);
  }
  float color1() const {
    return GetField<float>(VT_COLOR1, 0.1f);
  }
  float color2() const {
    return GetField<float>(VT_COLOR2, 0.1f);
  }
  float scale() const {
    return GetField<float>(VT_SCALE, 0.5f);
  }
  const flatbuffers::String *item_was_picked_words_type() const {
    return GetPointer<const flatbuffers::String *>(VT_ITEM_WAS_PICKED_WORDS_TYPE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ITEM_TYPE) &&
           verifier.VerifyString(item_type()) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<float>(verifier, VT_ITEM_DATA, 4) &&
           VerifyField<int16_t>(verifier, VT_TIME, 2) &&
           VerifyField<float>(verifier, VT_COLOR0, 4) &&
           VerifyField<float>(verifier, VT_COLOR1, 4) &&
           VerifyField<float>(verifier, VT_COLOR2, 4) &&
           VerifyField<float>(verifier, VT_SCALE, 4) &&
           VerifyOffset(verifier, VT_ITEM_WAS_PICKED_WORDS_TYPE) &&
           verifier.VerifyString(item_was_picked_words_type()) &&
           verifier.EndTable();
  }
};

struct itemBuilder {
  typedef item Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_item_type(flatbuffers::Offset<flatbuffers::String> item_type) {
    fbb_.AddOffset(item::VT_ITEM_TYPE, item_type);
  }
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(item::VT_NAME, name);
  }
  void add_item_data(float item_data) {
    fbb_.AddElement<float>(item::VT_ITEM_DATA, item_data, 10.0f);
  }
  void add_time(int16_t time) {
    fbb_.AddElement<int16_t>(item::VT_TIME, time, 10);
  }
  void add_color0(float color0) {
    fbb_.AddElement<float>(item::VT_COLOR0, color0, 0.9f);
  }
  void add_color1(float color1) {
    fbb_.AddElement<float>(item::VT_COLOR1, color1, 0.1f);
  }
  void add_color2(float color2) {
    fbb_.AddElement<float>(item::VT_COLOR2, color2, 0.1f);
  }
  void add_scale(float scale) {
    fbb_.AddElement<float>(item::VT_SCALE, scale, 0.5f);
  }
  void add_item_was_picked_words_type(flatbuffers::Offset<flatbuffers::String> item_was_picked_words_type) {
    fbb_.AddOffset(item::VT_ITEM_WAS_PICKED_WORDS_TYPE, item_was_picked_words_type);
  }
  explicit itemBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<item> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<item>(end);
    return o;
  }
};

inline flatbuffers::Offset<item> Createitem(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> item_type = 0,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    float item_data = 10.0f,
    int16_t time = 10,
    float color0 = 0.9f,
    float color1 = 0.1f,
    float color2 = 0.1f,
    float scale = 0.5f,
    flatbuffers::Offset<flatbuffers::String> item_was_picked_words_type = 0) {
  itemBuilder builder_(_fbb);
  builder_.add_item_was_picked_words_type(item_was_picked_words_type);
  builder_.add_scale(scale);
  builder_.add_color2(color2);
  builder_.add_color1(color1);
  builder_.add_color0(color0);
  builder_.add_item_data(item_data);
  builder_.add_name(name);
  builder_.add_item_type(item_type);
  builder_.add_time(time);
  return builder_.Finish();
}

inline flatbuffers::Offset<item> CreateitemDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *item_type = nullptr,
    const char *name = nullptr,
    float item_data = 10.0f,
    int16_t time = 10,
    float color0 = 0.9f,
    float color1 = 0.1f,
    float color2 = 0.1f,
    float scale = 0.5f,
    const char *item_was_picked_words_type = nullptr) {
  auto item_type__ = item_type ? _fbb.CreateString(item_type) : 0;
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto item_was_picked_words_type__ = item_was_picked_words_type ? _fbb.CreateString(item_was_picked_words_type) : 0;
  return fbs_bandai_siv_game_bin_input::Createitem(
      _fbb,
      item_type__,
      name__,
      item_data,
      time,
      color0,
      color1,
      color2,
      scale,
      item_was_picked_words_type__);
}

struct items_type FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef items_typeBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ITEMS_DATA = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::item>> *items_data() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::item>> *>(VT_ITEMS_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ITEMS_DATA) &&
           verifier.VerifyVector(items_data()) &&
           verifier.VerifyVectorOfTables(items_data()) &&
           verifier.EndTable();
  }
};

struct items_typeBuilder {
  typedef items_type Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_items_data(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::item>>> items_data) {
    fbb_.AddOffset(items_type::VT_ITEMS_DATA, items_data);
  }
  explicit items_typeBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<items_type> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<items_type>(end);
    return o;
  }
};

inline flatbuffers::Offset<items_type> Createitems_type(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::item>>> items_data = 0) {
  items_typeBuilder builder_(_fbb);
  builder_.add_items_data(items_data);
  return builder_.Finish();
}

inline flatbuffers::Offset<items_type> Createitems_typeDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::item>> *items_data = nullptr) {
  auto items_data__ = items_data ? _fbb.CreateVector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::item>>(*items_data) : 0;
  return fbs_bandai_siv_game_bin_input::Createitems_type(
      _fbb,
      items_data__);
}

inline const fbs_bandai_siv_game_bin_input::items_type *Getitems_type(const void *buf) {
  return flatbuffers::GetRoot<fbs_bandai_siv_game_bin_input::items_type>(buf);
}

inline const fbs_bandai_siv_game_bin_input::items_type *GetSizePrefixeditems_type(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<fbs_bandai_siv_game_bin_input::items_type>(buf);
}

inline bool Verifyitems_typeBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<fbs_bandai_siv_game_bin_input::items_type>(nullptr);
}

inline bool VerifySizePrefixeditems_typeBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<fbs_bandai_siv_game_bin_input::items_type>(nullptr);
}

inline void Finishitems_typeBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<fbs_bandai_siv_game_bin_input::items_type> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixeditems_typeBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<fbs_bandai_siv_game_bin_input::items_type> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace fbs_bandai_siv_game_bin_input

#endif  // FLATBUFFERS_GENERATED_ITEMSTYPE_FBS_BANDAI_SIV_GAME_BIN_INPUT_H_
