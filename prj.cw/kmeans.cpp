#include <ctime>   
#include <fstream> 
#include <iostream>
#include <sstream> 
#include <vector>
#include <cfloat>
#include <stdexcept>
#include <filesystem>

#include "include/algorithm/kmeans.hpp"


Point::Point(): 
    x(0.0), 
    y(0.0),
    cluster(-1),
    minDist(DBL_MAX) {}
    
Point::Point(double x, double y): 
    x(x), 
    y(y),
    cluster(-1),
    minDist(DBL_MAX) {}

double Point::distance(Point p) {
    return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
}


kMeans::kMeans() {
    clusters = 2;
}

kMeans::kMeans(int new_clusters) {
    if (0 > new_clusters || new_clusters > 10) {
        throw std::invalid_argument("'new_clusters' is out of range.");
    }
    clusters = new_clusters;
}

// std::string path, int column, std::vector<int> need_columns
void kMeans::read_csv(std::string path, std::vector<int> need_columns) {
    for(int i = 0; i < need_columns.size(); i++) need_columns[i]--;

    if (path.substr(path.size() - 4, 4) != ".csv"){
        throw std::invalid_argument(" File format is uncorrect.");
    }

    int columns = 0;
    std::string line;
    std::ifstream file(path);
    
    if (!file){
        throw std::invalid_argument("File for read can't be opened!");
    }
    // Read headers line
    getline(file, line);
    for (int i = 0; i < line.size(); i++) if (line[i] == ',') columns++;

    for (auto id : need_columns) {
        if (id > columns) {
            file.close();
            throw std::invalid_argument("Element in 'need_columns' is out of range (" + std::to_string(id + 1) + " > " + std::to_string(columns + 1) + ")");
        }
    }

    while (getline(file, line)) {
        std::stringstream lineStream(line);
        std::string bit;
        double x, y;
        int acc = 0;
        while (acc < columns){
            getline(lineStream, bit, ',');
            for (int i = 0; i < need_columns.size(); i++){
                if (acc == need_columns[i]) {
                    x = stod(bit);
                    break;
                }
                else if (acc == need_columns[i]) {
                    y = stod(bit);
                    break;
                }
            }
            acc++;
        }
        getline(lineStream, bit, '\n');
        if (acc == need_columns[need_columns.size() - 1]) {
            y = stod(bit);
        }
        points.push_back(Point(x, y));
    }
}

void kMeans::_centroid_intialization(){
    srand(time(0));  // need to set the random seed
    for (int i = 0; i < clusters; ++i) {
        centroids.push_back(points.at(rand() % 100));
    }
}

void kMeans::_assigning_points(){
    for (std::vector<Point>::iterator c = begin(centroids); c != end(centroids); ++c) {
        // quick hack to get cluster index
        int clusterId = c - begin(centroids);

        for (std::vector<Point>::iterator it = points.begin();
             it != points.end(); ++it) {
            Point p = *it;
            double dist = c->distance(p);
            if (dist < p.minDist) {
                p.minDist = dist;
                p.cluster = clusterId;
            }
            *it = p;
        }
    }
}

void kMeans::_computing_new_centroids(){
    std::vector<int> nPoints;
    std::vector<double> sumX, sumY;

    // Initialise with zeroes
    for (int j = 0; j < clusters; ++j) {
        nPoints.push_back(0);
        sumX.push_back(0.0);
        sumY.push_back(0.0);
    }
    // Iterate over points to append data to centroids
    for (std::vector<Point>::iterator it = points.begin(); it != points.end(); ++it) {
        int clusterId = it->cluster;
        nPoints[clusterId] += 1;
        sumX[clusterId] += it->x;
        sumY[clusterId] += it->y;

        it->minDist = DBL_MAX;  // reset distance
    }

    // Compute the new centroids
    for (std::vector<Point>::iterator c = begin(centroids); 
         c != end(centroids); ++c) {
        int clusterId = c - begin(centroids);
        c->x = sumX[clusterId] / nPoints[clusterId];
        c->y = sumY[clusterId] / nPoints[clusterId];
    }
}

void kMeans::output_file(std::vector<Point>& points, std::string output_path){
    
    if (!std::filesystem::exists("result")) std::filesystem::create_directory("result");
    std::ofstream myfile(output_path);

    if (!myfile) {
        throw std::invalid_argument("File for write can't be opened!");
    }

    if (output_path.substr(output_path.size() - 4, 4) != ".csv"){
        myfile.close();
        throw std::invalid_argument(" File format is uncorrect.");
    }

    myfile << "x,y,c" << std::endl;

    for (std::vector<Point>::iterator it = points.begin(); 
         it != points.end(); ++it) {
        myfile << it->x << "," << it->y << "," << it->cluster << std::endl;
    }
    myfile.close();
}

void kMeans::k_means_clustering(int epochs, std::string output_path){
    std::cout << "Start Clustering...\n";
    _centroid_intialization();
    std::cout << "Initialization finished!\n";
    for (int i = 0; i < epochs; i++){
        _assigning_points();
        _computing_new_centroids();
    }
    output_file(points, output_path);
    std::cout << "Clustering success!\n";
}
