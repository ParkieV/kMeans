#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cfloat>


struct Point{
    double x, y;
    int cluster;
    double minDist;
    Point();
    /**
     * \detail Присваивает координаты по оси X и оси Y.
     * \param[in] x координата точки по оси X.
     * \param[in] y координата точки по оси Y.
     */
    Point(double x, double y);
    double distance(Point p);
};

class kMeans {
    private:
        std::vector<Point> points;
        std::vector<Point> centroids;
        int columns;
        int clusters;

        /**
         * \detail Определяет первоначальные центроиды кластеров из данного набора.
         */
        void _centroid_intialization();
        /**
        * \detail вычисление, к какому кластеру пинадлежит каждая из точек.
        */
        void _assigning_points();/**
        * \detail Переопределение центроидов кластеров.
        */
        void _computing_new_centroids();
    
    public:
        kMeans();
        kMeans(const kMeans&) = default;
        kMeans(kMeans&&) = default;
        ~kMeans() = default;
        /**
        * \detail Определение количества кластеров, на какое необходимо разбить точки.
        * \param[in] new_clusters количество кластеров.
        * \throw std::invalid_argument если 0 > new_clusters или new_clusters > 10.
        */
        kMeans(int new_clusters);
        /**
        * \detail Заполнение массива точек типа Point из файла.
        * \param[in] path путь до файла для чтения.
        * \param[in] y индекс необходимых для чтения колонок.
        * \throw std::invalid_argument если формат файла не .csv .
        * \throw std::invalid_argument если файл невозможно открыть.
        */
        void read_csv(std::string path = "project/include/datasets/dataset0001.csv", std::vector<int> need_columns = std::vector<int>{4, 5});
        /**
        * \detail Запись массива точек типа Point в файл.
        * \param[in] points массив точек типа Point.
        * \param[in] output_path путь до файла для вывода.
        * \throw std::invalid_argument если формат файла не .csv .
        * \throw std::invalid_argument если файл невозможно открыть.
        */
        void output_file(std::vector<Point>& points, std::string output_path = "result/output.csv");
        /**
        * \detail Алгоритм кластеризации K-means для массива точек типа Point и выводом в файл формата .csv .
        * \param[in] epochs количество итераций алгоритма.
        * \param[in] output_path путь до файла для вывода.
        */
        void k_means_clustering(int epochs = 1000, std::string output_path = "result/output.csv");
};

#endif