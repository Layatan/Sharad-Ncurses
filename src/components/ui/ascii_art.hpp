#pragma once
#include <string>
#include <string_view>
#include <locale>
#include <codecvt>

namespace asciiArt {

class AsciiArtwork{
  public:
    std::u32string artwork;
    int width = 0, height = 1;
  
    AsciiArtwork (std::string_view a = std::string_view{R"(Ascii Not Loaded!)"}) {
      std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convU8_U32;
      artwork = convU8_U32.from_bytes((std::string)a);

      int max = 0, curr = 0;
      for (char32_t c : artwork) {
        if (c == U'\n') {
          if (max < curr+1) max = curr+1; 
          curr = 0;
          height++;
        } else curr++;
      }
      max < curr+1 ? width = curr+1 : width = max;
    }
};

const AsciiArtwork logo = {(std::string_view{ 
//the first line alignement must be messed up. WEAKNESS OF THE FLESH WILL NOT DISPLEASE THE MACHINE GODS MUHAHAHAHAHAAH
  R"(                  ...  ..      . %%#+=:..
               ...+@@-. ..@@.     @.*. ..*=
              .@:..+%.. .@@#.@+. .:-@%.@@.%..
                .=@..@@@.+@@%@.%- .@.@@@@@.%.
          .@@@@@:...@..@@@@@+.@@.=--.%@@@@%.@.
        .%+.@@@*..%%@%#.@*..*@@@@@@@@@-@%.@@@.-@*..
      .:@.  .-@@@@@*.@@@@@@@@@..........:@@@@@@%#.%%..
    ..%..%@@@+@@..@@@@@@@..@+ .#@%-....@..  #%.#@@@%.%.
   .@-...*%@@%@.@.. .@@.  .@@. .  .:%@%-.  .@.   @@@@%.@+.
   ......-%@.@@-:.@-.@..+@@@@@@@@@@@@@@@@@@..    =@...@%.@.
       ..:@.@@%@.. .@@@@#..           .@@@@@@@@@@@..  ..@.#+.
   .@@...*@%@+.@@@@@@#..%@@@@..       *@.@@@@...@%@@:.  .@*#..
  @..@@@@@.-%. .%@@@@@@:....:===-.....@#.%%:....@@..@@@. .@.@ .
  @.  .%@@.@.    ..@@@@@@@@@@%#++#@@@@@@@@@@::@@:..:@.@@..%.@..
  @..:@%@%@%   :@@%.    .@..@%-::+@%@@%+...@@@@@%::%.#:%@@.+:.
  @.@-.@.@@#   @@@%.    :@.@               -@@@:.+.@..%-..@
  @..@.%.@@%.   ...   .@@@%..@%@..            ....%.=@%%..
  @@:#:=%%.@@@@@@@@@@@@%..%@@@%@=..@%-.
  %.@.@@@@.@@..=#%#=..     %@:-#@@@@%..%%..
   @... .@%.@..  ..%@%.    @@.  .:%.@@@%..@@.
   .@...@@+.-%-   .@@@. .@%..@@..@@ . .:@%%.-@..
    .@:.@.+%%.#%@*...-@@@..    .@@.      .@@%.##.
     .%:.%...:@@..-@@@@@@@%@@@%@#..      . .%@..@.
      .@@.     . .*@@...      ..:@@@#   .@@@%@@#.@.
               .@..@@=..@@@#...   ..%@@. @@ .-@@@.%.
             .@.%@..@.%@...@@*-+@%+.. %@.%#   .@@:.@.
            *:.@:..=@%..%..       ..%..*@+.    .@@.@..
           +..@..%.@@ :*             .%.#@     .@@:.*
          .@.@@:#@@@..#               ::.@+  .%..@% @
         .%.%@ @@.@%.@.               .@ @@..@@@.%@ @
         .@.%= ...@%.@.                @ %%%@..%%%@ @
         .@.@@@....%..:               .#.@@..#..@@:.*
         .@.%@.@@+.@@.%..            .%.%% .@@@.@@.%..
          :# @....%@@@..@.        ..%+.%@.  ....%:.%.
           @.:@.=%  .@@*..@%.....*@..-@%.     .@@.%.
           .@.:@% .@@..@@@*.......=@@%       .@%.@.
           ..@..@*.@@..%...@@@@@@#..        .@..@.
              *@.=@.. -@..@@..@# .@@@%    .@*.@=.
               .@@. @%#@..@%. :@:.:@%. .@@..%% .
                ..*@...@%@:.....@@#.%@@..=@=.
                   ..+@@:............+@%:
                         ..%%%%%%@*..
                           '''''''''
)"})};
const AsciiArtwork title = {(std::string_view{
  R"( ¥¥¥¥¥¥\  ¥¥\                                          ¥¥\
¥¥  __¥¥\ ¥¥ |                                         ¥¥ |
¥¥ /  \__|¥¥¥¥¥¥¥\   ¥¥¥¥¥¥\   ¥¥¥¥¥¥\  ¥¥¥¥¥¥\   ¥¥¥¥¥¥¥ | ¥¥ \
\¥¥¥¥¥¥\  ¥¥  __¥¥\  \____¥¥\ ¥¥  __¥¥\ \____¥¥\ ¥¥  __¥¥ | \_\/
 \____¥¥\ ¥¥ |  ¥¥ | ¥¥¥¥¥¥¥ |¥¥ |  \__|¥¥¥¥¥¥¥ |¥¥ /  ¥¥ |
¥¥\   ¥¥ |¥¥ |  ¥¥ |¥¥  __¥¥ |¥¥ |     ¥¥  __¥¥ |¥¥ |  ¥¥ | ¥¥ \
\¥¥¥¥¥¥  |¥¥ |  ¥¥ |\¥¥¥¥¥¥¥ |¥¥ |     \¥¥¥¥¥¥¥ |\¥¥¥¥¥¥¥ | \_\/
 \______/ \__|  \__| \_______|\__|      \_______| \_______|

              ███╗  ██╗                     ████████████████████╗
              ████╗ ██║                     ╚═════╝╚═════╝╚═════╝
              ██╔██╗██║██████╗██╗ ██╗██████╗ █████╗ ████╗ █████╗
              ██║╚████║██╔═══╝██║ ██║██╔═══╝ ╚══██║ ╚═══╝ ██╔══╝
              ██║ ╚███║██████╗██████║██║    ██████║██████╗██████╗
              ╚═╝  ╚══╝╚═════╝╚═════╝╚═╝    ╚═════╝╚═════╝╚═════╝
)"})};

const AsciiArtwork titleCursor = {(std::string_view{
  R"(████████╗
████████║
████████║
████████║
████████║
╚═══════╝)"
})};

const AsciiArtwork credits = {(std::string_view{
  R"(Made (with love&hate) by:
   Bai Omar Laye

   Inspired (and permitted) By:
   ProHaller's Sharad Ratatui - tnx fam
   & contributors

   Powered By:
   Ncurses
   & contributors

   Remember its always more fun with
   friends, go out there and join a
   group. Try out these subreddits:
   r/Shadowrun
   r/ShadowrunHaven
   they get annoying about the edition
   to play so just go 5e.
   
   Oh and play the HBS Shadowrun
   Trilogy and Cirion's Trilogy After.
   "copyright: 2024")"
})};

} //namespace end