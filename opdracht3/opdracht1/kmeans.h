#ifndef kmeans_H
#define kmeans_H

#include "Image.h";
#include "linkedList.h";


class kMeans : public Image{
private:
	int Clusters = 0;
public:
	Pixel ** MedianData;
	kMeans();
	kMeans(Image * img) : Image(*img){}
	kMeans(const Image& rhs);
	~kMeans();
	void clusteringByKMeans(int Clusters, int MAX_iterations);
};

#endif