// Command-line argument parsing library
//
// Pavel Pilař, Michael Pokorný
//
// The API is deliberately designed to be very simple for the user.
// The user only needs to register options via AddBool/Int/String/Enum and call
// Parse in main().
//
// Strictly speaking, the assignment asked to implement "options that aren't
// allowed to have a value". This makes sense with booleans, but since it would
// be impossible to pass a non-bool option without a value, this makes no
// sense for non-bools. Thus we decided to skip this part of the assignment.
//
// TODO: But it might make sense to only allow "-x" for booleans, while
// not allowing "-x false"/"-x=true". Perhaps we should add this as a special
// mode for AddBool?
// TODO: is it a "flag", or an "option"? unify nomenclature

#include <string>
#include <vector>
#include <utility>

// All functions and types of the library are within the args namespace.
namespace args {

// TODO: "ls -laxHrtlm"

// Prints out help about all registered options to standard output.
void Usage() {
}

// Parse gets a pointer to argc and argv as given to main(), parses out
// options and modifies argc and argv to contain only nonoption arguments.
// If option parsing fails, Parse prints out an error message describing
// the problem and exits with exit code 1.
// If no long option named "--help" or short option named "-h" is registered,
// passing either of these options calls Usage and exits with exit code 0.
//
// This method doesn't throw exceptions or indicate failure via a return value
// to make the most common use case simpler (most programs don't need anything
// but the default behavior).
//
// Intended usage:
//   int main(int argc, char** argv) {
//     flags::Parse(&argc, &argv);
//     // argc, argv now only contain non-option arguments.
//   }
void Parse(int* argc, char*** argv) {
}

// TODO: Should this display help and quit on --help?
// TODO: Is it OK to just say "flags OK" / "flags not OK"?
bool TryParse(int argc, const char** argv) {
}

const std::vector<std::string>& GetNonoption() {
  return *(new std::vector<std::string>);
}

// The args::internal namespace contains implementation details that need
// to be visible in the header due to C++ rules.
namespace internal {

struct FlagName {
 public:
  FlagName(const std::string& name) {}
  FlagName(const char* name) {}
  FlagName(char short_name) {}
};

template<typename T>
class Flag {
 public:
  virtual T get() = 0;
  virtual bool present() = 0;

  // Our initial idea was to also include 'operator T()' what would call .get().
  // Eventually, we decided against that, mostly because even with this
  // operator, flags would not be fully usable as their underlying types.
  // For example, this wouldn't work due to C++ type inference rules:
  //     cout << my_string_flag << endl;
  // We would need to write:
  //     cout << my_string_flag.get() << endl;
  // On the other hand, this would work:
  //     string my_flag_value = my_string_flag;
  //
  // We decided to remove the 'operator T()' to make the API more consistent
  // (always 'flag.get()'). Also, because 'get()' throws exceptions when
  // accessing nonpresent flags, 'operator T()' could throw exceptions as well.
  // 'string value = my_string_flag;', however, looks innocent.
  // 'string value = my_string_flag.get();' makes the possibility of failure
  // more explicit.

  void alias(const FlagName& name) {
  }
  void alias(const std::initializer_list<FlagName>& names) {
  }

  // TODO: mozna explicitni set?
};

// TODO: BoolFlag, IntFlag

class StringFlag : public Flag<std::string> {
 public:
  std::string get() override {
    return "";
  }
  bool present() override {
    return false;
  }
};

class EnumFlag : public StringFlag {};

}  // namespace internal

// In the context of the library, flags are represented as internal::Flag
// subclasses (internal::BoolFlag, internal::IntFlag, ...).
// These internal names are exposed as args::Bool, args::Int, ... to make usage
// simpler (e.g. "args::String my_flag" instead of "args::StringFlag my_flag").
typedef internal::StringFlag String;
typedef internal::EnumFlag Enum;

// struct {
//   args::StringFlag my_string;
// } Options;
//
// args::AddString(&Options.my_string, "output", args::REQUIRED, "this flag is useful");
// Options.my_string.alias('o', "output_thing")
// args::Alias(&Options.my_string, 'o', "output_thing");
const bool REQUIRED = true;
const bool OPTIONAL = false;
internal::StringFlag& AddString(internal::StringFlag*,
                                const internal::FlagName& name,
                                bool required,
                                const std::string& documentation) {
  return *(new internal::StringFlag);
}

// TODO: namapovat enum na C++ enum?

// TODO: Alias should be either templated over Flag derived classes, or
// copied for all flag types.
// OR - we could also make it 'void Alias(Flag*, FlagName),', which would
// limit the fluentness of the API. I think it would be a good idea.
// Less complexity that way.
internal::StringFlag& Alias(internal::StringFlag*,
                            const internal::FlagName& name) {
}
internal::StringFlag& Alias(internal::StringFlag*,
                            const std::initializer_list<internal::FlagName>&
                                names) {
}

internal::EnumFlag& AddEnum(internal::EnumFlag*,
                            const internal::FlagName& name, bool required,
                            const std::initializer_list<std::string>&
                                allowed_values,
		            const std::string& documentation) {
  return *(new internal::EnumFlag);
}
internal::EnumFlag& AddEnum(internal::EnumFlag*,
                            const internal::FlagName& name, bool required,
                            std::initializer_list<const char*> allowed_values,
		            const std::string& documentation) {
  return *(new internal::EnumFlag);
}

// TODO: boolean, int

}  // namespace args
