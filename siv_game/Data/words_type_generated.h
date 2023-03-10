// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_WORDSTYPE_FBS_BANDAI_SIV_GAME_BIN_INPUT_H_
#define FLATBUFFERS_GENERATED_WORDSTYPE_FBS_BANDAI_SIV_GAME_BIN_INPUT_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 22 &&
              FLATBUFFERS_VERSION_MINOR == 12 &&
              FLATBUFFERS_VERSION_REVISION == 6,
             "Non-compatible flatbuffers version included");

namespace fbs_bandai_siv_game_bin_input {

struct word;
struct wordBuilder;

struct words_type;
struct words_typeBuilder;

struct word FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef wordBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_WORD_TYPE = 4,
    VT_JAPANESE = 6,
    VT_CANTONESE = 8,
    VT_ENGLISH = 10,
    VT_UNKNOW = 12
  };
  const flatbuffers::String *word_type() const {
    return GetPointer<const flatbuffers::String *>(VT_WORD_TYPE);
  }
  const flatbuffers::String *japanese() const {
    return GetPointer<const flatbuffers::String *>(VT_JAPANESE);
  }
  const flatbuffers::String *cantonese() const {
    return GetPointer<const flatbuffers::String *>(VT_CANTONESE);
  }
  const flatbuffers::String *english() const {
    return GetPointer<const flatbuffers::String *>(VT_ENGLISH);
  }
  const flatbuffers::String *unknow() const {
    return GetPointer<const flatbuffers::String *>(VT_UNKNOW);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_WORD_TYPE) &&
           verifier.VerifyString(word_type()) &&
           VerifyOffset(verifier, VT_JAPANESE) &&
           verifier.VerifyString(japanese()) &&
           VerifyOffset(verifier, VT_CANTONESE) &&
           verifier.VerifyString(cantonese()) &&
           VerifyOffset(verifier, VT_ENGLISH) &&
           verifier.VerifyString(english()) &&
           VerifyOffset(verifier, VT_UNKNOW) &&
           verifier.VerifyString(unknow()) &&
           verifier.EndTable();
  }
};

struct wordBuilder {
  typedef word Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_word_type(flatbuffers::Offset<flatbuffers::String> word_type) {
    fbb_.AddOffset(word::VT_WORD_TYPE, word_type);
  }
  void add_japanese(flatbuffers::Offset<flatbuffers::String> japanese) {
    fbb_.AddOffset(word::VT_JAPANESE, japanese);
  }
  void add_cantonese(flatbuffers::Offset<flatbuffers::String> cantonese) {
    fbb_.AddOffset(word::VT_CANTONESE, cantonese);
  }
  void add_english(flatbuffers::Offset<flatbuffers::String> english) {
    fbb_.AddOffset(word::VT_ENGLISH, english);
  }
  void add_unknow(flatbuffers::Offset<flatbuffers::String> unknow) {
    fbb_.AddOffset(word::VT_UNKNOW, unknow);
  }
  explicit wordBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<word> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<word>(end);
    return o;
  }
};

inline flatbuffers::Offset<word> Createword(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> word_type = 0,
    flatbuffers::Offset<flatbuffers::String> japanese = 0,
    flatbuffers::Offset<flatbuffers::String> cantonese = 0,
    flatbuffers::Offset<flatbuffers::String> english = 0,
    flatbuffers::Offset<flatbuffers::String> unknow = 0) {
  wordBuilder builder_(_fbb);
  builder_.add_unknow(unknow);
  builder_.add_english(english);
  builder_.add_cantonese(cantonese);
  builder_.add_japanese(japanese);
  builder_.add_word_type(word_type);
  return builder_.Finish();
}

inline flatbuffers::Offset<word> CreatewordDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *word_type = nullptr,
    const char *japanese = nullptr,
    const char *cantonese = nullptr,
    const char *english = nullptr,
    const char *unknow = nullptr) {
  auto word_type__ = word_type ? _fbb.CreateString(word_type) : 0;
  auto japanese__ = japanese ? _fbb.CreateString(japanese) : 0;
  auto cantonese__ = cantonese ? _fbb.CreateString(cantonese) : 0;
  auto english__ = english ? _fbb.CreateString(english) : 0;
  auto unknow__ = unknow ? _fbb.CreateString(unknow) : 0;
  return fbs_bandai_siv_game_bin_input::Createword(
      _fbb,
      word_type__,
      japanese__,
      cantonese__,
      english__,
      unknow__);
}

struct words_type FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef words_typeBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_WORDS_DATA = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::word>> *words_data() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::word>> *>(VT_WORDS_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_WORDS_DATA) &&
           verifier.VerifyVector(words_data()) &&
           verifier.VerifyVectorOfTables(words_data()) &&
           verifier.EndTable();
  }
};

struct words_typeBuilder {
  typedef words_type Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_words_data(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::word>>> words_data) {
    fbb_.AddOffset(words_type::VT_WORDS_DATA, words_data);
  }
  explicit words_typeBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<words_type> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<words_type>(end);
    return o;
  }
};

inline flatbuffers::Offset<words_type> Createwords_type(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::word>>> words_data = 0) {
  words_typeBuilder builder_(_fbb);
  builder_.add_words_data(words_data);
  return builder_.Finish();
}

inline flatbuffers::Offset<words_type> Createwords_typeDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::word>> *words_data = nullptr) {
  auto words_data__ = words_data ? _fbb.CreateVector<flatbuffers::Offset<fbs_bandai_siv_game_bin_input::word>>(*words_data) : 0;
  return fbs_bandai_siv_game_bin_input::Createwords_type(
      _fbb,
      words_data__);
}

inline const fbs_bandai_siv_game_bin_input::words_type *Getwords_type(const void *buf) {
  return flatbuffers::GetRoot<fbs_bandai_siv_game_bin_input::words_type>(buf);
}

inline const fbs_bandai_siv_game_bin_input::words_type *GetSizePrefixedwords_type(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<fbs_bandai_siv_game_bin_input::words_type>(buf);
}

inline bool Verifywords_typeBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<fbs_bandai_siv_game_bin_input::words_type>(nullptr);
}

inline bool VerifySizePrefixedwords_typeBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<fbs_bandai_siv_game_bin_input::words_type>(nullptr);
}

inline void Finishwords_typeBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<fbs_bandai_siv_game_bin_input::words_type> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedwords_typeBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<fbs_bandai_siv_game_bin_input::words_type> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace fbs_bandai_siv_game_bin_input

#endif  // FLATBUFFERS_GENERATED_WORDSTYPE_FBS_BANDAI_SIV_GAME_BIN_INPUT_H_
