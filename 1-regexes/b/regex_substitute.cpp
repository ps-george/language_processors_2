#include <iostream>
#include <string>
#include <regex>

int main(int argc, char *argv[])

{
    // Read regex and subtituion string
    std::regex rgx;
    std::string sbs;

    if (argc==1){
      std::cerr << "No regex or subsitution string provided." << std::endl;
      return 1;
    }
    if (argc>1 && argc!=3){
      rgx = argv[1];
      std::cerr << "No subtitution string provided." << std::endl;
      return 1;
    }
    
    if (argc==3){
      rgx = argv[1];
      sbs = argv[2];
    }
    else {
      std::cerr << "Too many arguments." << std::endl;
      return 1;
    }
    
    std::string tmp;
    while( std::getline(std::cin, tmp) ){
      //   Apply the regex and print out the new string
      std::cout << std::regex_replace(tmp,rgx,sbs,std::regex_constants::format_sed) << std::endl;
    }
    return 0;
}
