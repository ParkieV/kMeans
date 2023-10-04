#include <iostream>
#include <string>
#include <vector>
#include "include/algorithm/kmeans.hpp"
#include "include/output/latex.hpp"

int main() {
    bool complete_program = false;
    while (!complete_program) {
    bool complete_flag = false;
        std::string input;
        std::string read_file_path;
        kMeans* algo;
        while (!complete_flag) {
            try {
                std::cout << "\nWrite number of cluster (max is 10): ";
                std::cin >> input;
                if (input == "\\q"){
                    return 0;
                }
                int clusters = std::stoi(input);
                if (clusters > 10) {
                    throw std::invalid_argument("'new_clusters' is out of range.");
                }
                algo = new kMeans(clusters);
                complete_flag = true;
            }
            catch (std::invalid_argument& e) {
                std::cerr << e.what() << std::endl;
                complete_flag = false;
            }
            catch(std::out_of_range& e){
                std::cerr << e.what() << std::endl;
                complete_flag = false;
            }
        }

        complete_flag = false;
        outputLatex* outputFile;
        while (!complete_flag) {
            try {
                std::cout << "\nWrite path to file for read in .csv format: ";
                std::cin >> input;
                if (input == "\\q"){
                    return 0;
                }
                read_file_path = input;

                if (read_file_path.substr(read_file_path.size() - 4, 4) != ".csv"){
                    throw std::invalid_argument(" File format is uncorrect.");
                }

                std::ifstream file(read_file_path);
                if (!file){
                    throw std::invalid_argument("File for read can't be opened!");
                }
                file.close();
                std::cout << "\nWrite number of reading column (separated by a space): ";
                std::cin.get();
                std::getline(std::cin, input);
                if (input == "\\q"){
                    return 0;
                }
                std::vector<int> need_columns;
                int l = 0, r = 0;
                while (r < input.length()) {
                    if (input[r] == ' '){
                        need_columns.push_back(std::stoi(input.substr(l, r - l)));
                        l = r + 1;
                    }
                    r++;
                }
                need_columns.push_back(std::stoi(input.substr(l, r - l)));
                if (need_columns.size() != 2) {
                    throw std::out_of_range("number of columns is out of range");
                }
                algo->read_csv(read_file_path, need_columns);
                algo->k_means_clustering(1000);
                outputFile = new outputLatex();
                outputFile->writeLatex();
                complete_flag = true;
                delete[] algo;
                delete[] outputFile;
            }
            catch (std::invalid_argument& e) {
                std::cerr << e.what() << std::endl;
                complete_flag = false;
            }
            catch(std::out_of_range& e){
                std::cerr << e.what() << std::endl;
                complete_flag = false;
            }
        }

        complete_flag = false;

        while (!complete_flag) {
            std::cout<<"\nTry again? (Y/N): ";
            std::cin >> input;
            if (input == "Y" || input == "y") {
                complete_flag = true;
            }
            else if (input == "N" || input == "n") {
                complete_flag = true;
                complete_program = true;
            }
        }
    }
}