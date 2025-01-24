#include <string>
#include <iostream>

namespace Nodes
{
std::string exprtk_to_cpp(std::string exprtk) noexcept;
}

int main(int argc, char** argv)
{
    std::string content;

    while(!std::cin.eof()) {
        char arr[1024];
        std::cin.read(arr,sizeof(arr));
        int s = std::cin.gcount();
        content.append(arr,s);
    }
  std::cerr << Nodes::exprtk_to_cpp(content) << std::endl;
}