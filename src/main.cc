#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Welcome to chessengine!\n";
    for (int i = 1; i < argc; i++)
    {
        std::cout << argv[i] << std::endl;
    }
}