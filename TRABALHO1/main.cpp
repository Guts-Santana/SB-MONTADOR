#include "MONTADOR.hpp"
#include "LIGADOR.hpp"
#include "pre_processamento.hpp"

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 1 && argc != 3 && argc != 4)
        {
            std::cerr << "Error opening file. See ./montador -help to see available commands." << std::endl;
            return 1;
        }

        if (argc == 1)
        {
            std::string flag = argv[1];
            if (flag == "-help")
            {
                std::cout << "Available commands:\n";
                std::cout << "./montador -o <filename> -> Assembles the file\n";
                std::cout << "./montador -p <filename> -> Pre-Processes the file\n";
                std::cout << "./montador -l <filename1> <filename2> -> Links the files\n";
                std::cout << "./montador -h -> Shows available commands\n";
                std::cout << "OR\n";
                std::cout << "Run make as file=<filename> to pre-process and assemble the file.'\n";
                std::cout << "Run make link file1=<filename1> file2=<filename2> to pre-process, assemble, and link the files.'\n";
                return 0;
            }
            else
            {
                std::cerr << "Error opening file. See ./montador -help to see available commands." << std::endl;
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
                std::cerr << "Error opening file. See ./montador -help to see available commands." << std::endl;
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
                std::cerr << "Error opening file. See ./montador -help to see available commands." << std::endl;
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
