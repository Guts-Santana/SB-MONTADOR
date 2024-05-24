#include "MONTADOR.hpp"
#include "LIGADOR.hpp"
#include "pre_processamento.hpp"

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2 && argc != 3 && argc != 4)
        {
            std::cerr << "Error opening file. See ./montador --help to see available commands." << std::endl;
            return 1;
        }

        if (argc == 2)
        {
            std::string flag = argv[1];
            std::cout << flag << std::endl;
            if (flag == "--help" || flag == "-h")
            {
                std::cout << std::endl;
                std::cout << "Usage: ./montador [OPTION] [FILENAME]" << std::endl;
                std::cout << std::endl;
                std::cout << " > Available commands:" << std::endl;
                std::cout << std::endl;
                std::cout << "./montador -o <filename> -> Assembles the file" << std::endl;
                std::cout << "./montador -p <filename> -> Pre-Processes the file" << std::endl;
                std::cout << "./montador -l <filename1> <filename2> -> Links the files together" << std::endl;
                std::cout << "./montador -h -> Shows available commands" << std::endl;
                std::cout << std::endl;
                std::cout << "> Or using 'Make'" << std::endl;
                std::cout << std::endl;
                std::cout << "Run make as file=<filename> To pre-process and assemble the file." << std::endl;
                std::cout << "Run make link file1=<filename1> file2=<filename2> To pre-process, assemble, and link the files." << std::endl;
                return 0;
            }
            else
            {
                std::cerr << "Error opening file. See ./montador --help to see available commands." << std::endl;
                return 1;
            }
        }
        else if (argc == 3)
        {
            std::string flag = argv[1];
            std::string filename = argv[2];
            if (flag == "-o")
            {
                Assembler assembler;
                try
                {
                    assembler.ReadFile(filename);
                }
                catch (const std::invalid_argument &e)
                {
                    std::cerr << e.what() << '\n';
                    std::cerr << "At line: " << assembler.line_counter << '\n';
                    return 1;
                }

                assembler.WriteFile(filename);
                return 0;
            }
            else if (flag == "-p")
            {
                LEITURA(filename);
                return 0;
            }
            else
            {
                std::cerr << "Error opening file. See ./montador --help to see available commands." << std::endl;
                return 1;
            }
        }
        else if (argc == 4)
        {
            std::string flag = argv[1];
            std::string filename1 = argv[2];
            std::string filename2 = argv[3];
            if (flag == "-l")
            {
                Linker linker;
                linker.linkFiles(filename1, filename2);
                return 0;
            }
            else
            {
                std::cerr << "Error opening file. See ./montador --help to see available commands." << std::endl;
                return 1;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
