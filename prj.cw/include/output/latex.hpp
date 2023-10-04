#ifndef LATEX_HPP
#define LATEX_HPP

#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <sstream>
#include <ctime>   
#include <fstream> 
#include <iostream>
#include <cmath>
#include <filesystem>

class Dot {
    private:
        std::map<int, std::string> color = {{0, "blue"},
                                            {1, "green"},
                                            {2, "yellow"},
                                            {3, "red"},
                                            {4, "purple"},
                                            {5, "brown"},
                                            {6, "orange"},
                                            {7, "pink"},
                                            {8, "black"},
                                            {9, "lime"}};

    public:

        std::tuple<double, double> coord = {0.0, 0.0};
    
        Dot(){
            coord = std::make_tuple(0, 0);
        }

        /**
        * \detail Представляет тип tuple из двух элемент в тип string
        * \param[in] coord тип tuple, состоящий из двух элементов.
        */
        std::string convertToString(std::tuple<double, double> coord){
            return std::to_string(std::get<0>(coord)) + "," + std::to_string(std::get<1>(coord));
        }

        /**
        * \detail Присваивает координаты по оси X и оси Y.
        * \param[in] color_id индентификатор цвета точки.
        * \param[in] new_coord тип tuple, состоящий из двух элементов (координаты точки по оси X и по оси Y).
        * \throw std::invalid_argument если 0 > color_id или color_id >= 10.
        */
        std::string outputPoint(int color_id, std::tuple<double, double> new_coord){
            if (0 > color_id || color_id >= 10) {
                throw std::invalid_argument("Color ID is out of range.");
            }
            return "\\draw[" + this->color[color_id] + "] (" + this->convertToString(new_coord) + ") circle (2pt);\n\\fill[" + this->color[color_id] + "] (" + this->convertToString(new_coord) + ") circle (2pt);\n";
        }
};


class outputLatex{
    private:
        std::vector<std::string> header = {"\\documentclass{article}\n\\usepackage{tkz-base}\n\\usepackage{tkz-euclide}\n\\begin{document}\nXY diagram (scale 1:", ")\\begin{figure}[h]\n\\begin{tikzpicture}\n\\tkzInit[xmax=10,ymax=10,xmin=0,ymin=0]\n\\tkzAxeXY\n"};
        std::string footer = "\\end{tikzpicture}\n\\end{figure}\n\\end{document}\n";
        std::string body = "";
        std::string file_path_input = "";
        std::string file_path_output = "";
    
    public:

        outputLatex(){
            file_path_input = "result/output.csv";
            file_path_output = "result/output.tex";
        }
        
        outputLatex(const outputLatex&) = default;
        outputLatex(outputLatex&&) = default;
        ~outputLatex() = default;
        /**
        * \detail Определение пути к файлу ввода в формате .csv .
        * \param[in] new_file_input путь к файлу ввода.
        */
        outputLatex(std::string new_file_input){
            file_path_input = new_file_input;
            file_path_output = "result/output.tex";
        }

        /**
        * \detail Определение путей к файлу ввода в формате .csv и файлу вывода в формате .tex .
        * \param[in] new_file_input путь к файлу ввода.
        * \param[in] new_file_output путь к файлу вывода.
        */
        outputLatex(std::string new_file_input, std::string new_file_output){
            file_path_input = new_file_input;
            file_path_output = new_file_output;
        }

        /**
        * \detail создание представления массива точек типа Point в код Latex'a.
        * \throw std::invalid_argument если формат файла не .csv .
        * \throw std::invalid_argument если файл невозможно открыть.
        */
        void make_body(){

            Dot point;
            int columns = 0;
            std::string line;
            std::ifstream file(file_path_input);
        
            if (!file){
                throw std::invalid_argument("File can't be opened.");
            }

            if (file_path_input.substr(file_path_input.size() - 4, 4) != ".csv"){
                file.close();
                throw std::invalid_argument(" File format is uncorrect.");
            }

            // Read headers line
            getline(file, line);

            for (int i = 0; i < line.size(); i++) if (line[i] == ',') 
                columns++;
            std::vector<std::vector<double>> points;
            double max_num = -1.0;
            int acc_points = 0;
            double x, y;
            while (getline(file, line)) {
                std::stringstream lineStream(line);
                std::string bit;
                int acc = 0;
                while (acc < columns){
                    getline(lineStream, bit, ',');
                    if (acc == 0) {
                        x = std::stod(bit);
                        if (x > max_num){
                            max_num = x;
                        }
                    }
                    else if (acc == 1) {
                        y = std::stod(bit);
                        if (y > max_num){
                            max_num = y;
                        }
                    }
                    acc++;
                }
                double color_id = 0;
                getline(lineStream, bit, '\n');
                if (acc == columns) {
                    color_id = stod(bit);
                }
                points.push_back(std::vector<double>());
                points[points.size() - 1].push_back(color_id);points[points.size() - 1].push_back(x);points[points.size() - 1].push_back(y);
            }
            max_num = max_num / 10.0;
            std::string strnum = std::to_string(max_num);
            int i = strnum.length() - 1;
            while (strnum[i] == '0') i--;
            if (strnum[i] == '.')
                strnum.erase(i, strnum.length() - i + 1);
            else
                strnum.erase(i + 1, strnum.length() - i);
            header.insert(header.begin() + 1, strnum);
            for (int i = 0; i < points.size(); i++) {
                points[i][1] = points[i][1] / max_num;
                points[i][2] = points[i][2] / max_num;
                body += point.outputPoint(points[i][0], std::make_tuple(points[i][1], points[i][2]));
            }
            std::cout << "Body finished!\n";
        }

        /**
        * \detail Ввод кода в файл вывода формате .tex .
        * \throw std::invalid_argument если формат файла не .tex .
        * \throw std::invalid_argument если файл невозможно открыть.
        */
        void writeLatex(){
            make_body();
            
            if (!std::filesystem::exists("result")) std::filesystem::create_directory("result");
            std::ofstream out(file_path_output);

            if (!out){
                    throw std::invalid_argument("File can't be opened.");
            }

            if (file_path_output.substr(file_path_output.size() - 4, 4) != ".tex"){
                out.close();
                throw std::invalid_argument(" File format is uncorrect.");
            }

            std::string code = header[0]+header[1]+header[2]+body+footer;
            out << code;
            out.close();
            std::cout << "Output finished!\n";
        }
};

#endif