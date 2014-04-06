#include "kmeans.h"
#include "linkedList.h"


using namespace std;
kMeans::kMeans(const Image& rhs) : Image(&rhs){}
kMeans::~kMeans(){}

void kMeans::clusteringByKMeans(int clusters, int MAX_iterations = 1){
	cout << "Applying K-Means clustering" << endl;

	/*
	init: choose k random pixel values as means
	while (!stop)
		for (int i in 1 : img_w*img_h)
			assign each pixel(from image data) to the nearest mean
		endfor
		for (int j in 1 : k)
			mean of cluster j ← mean of all
			pixels with label j
		endfor
	endwhile
	final: change all pixel's colors to match color of the cluster mean
	*/

	//Linkedlist cause hardmode...
	LinkedList<int> list;
	LinkedList<int> **ListOfClusters = new LinkedList<int>*[clusters];
	LinkedList<int> **ListOfRGB = new LinkedList<int>*[clusters];
	int * clusterSegmentCount = new int[clusters];

	int tempx = 0;
	int tempy = 0;
	int h = getHeight();
	int w = getWidth();
	int allPixels = w*h;
	int stop = 0;

	int r = 0;
	int g = 0;
	int b = 0;
	int red;
	int green;
	int blue;
	int REDCount = 0;
	int BLUECount = 0;
	int GREENCount = 0;
	int oldRed = 0;
	int oldGreen = 0;
	int oldBlue = 0;
	int rgbCount = 0;
	int clusterCount = 0;
	int closest = 9000;
	int distance = 0;
	int tempCluster = 0;
	int iterations = 0;
	int x_cord = 0;
	int y_cord = 0;
	int red_2 = 0;
	int green_2 = 0;
	int blue_2 = 0;
	for (int i = 0; i < clusters; ++i) {
		ListOfClusters[i] = new LinkedList<int>;
		ListOfRGB[i] = new LinkedList<int>;
		ListOfRGB[i]->add(0, 0, 0,0,0);
		clusterSegmentCount[i] = 0;
		tempx = rand() % w;
		tempy = rand() % h;
		list.add(tempx, tempy, i);
	}



	//INIT
	list.print();
	while (!stop){
		iterations++;
		for (int x = 0; x < w; x++){
			if (x % (w/10) == 0){
				cout << (((double)x / (double)w) * (double)100) << "%" << endl;
			}
			for (int y = 0; y < h; y++){
				tempCluster = 0;
				clusterCount = 0;
				closest = 9000;
				red = this->getRed(x, y);
				green = this->getGreen(x, y);
				blue = this->getBlue(x, y);
				while (clusterCount < list.count()){
					if (iterations == 1){
						r = red - (int)this->getRed(list.getValueWithID(clusterCount, 1), list.getValueWithID(clusterCount, 2));
						g = green - (int)this->getGreen(list.getValueWithID(clusterCount, 1), list.getValueWithID(clusterCount, 2));
						b = blue - (int)this->getBlue(list.getValueWithID(clusterCount, 1), list.getValueWithID(clusterCount, 2));
					}
					else {
						r = red - ListOfRGB[clusterCount]->getValueWithID(0, 1);
						g = green - ListOfRGB[clusterCount]->getValueWithID(0, 2);
						b = blue - ListOfRGB[clusterCount]->getValueWithID(0, 3);
					}
					
					r = (r < 0) ? r*-1 : r;
					g = (g < 0) ? g*-1 : g;
					b = (b < 0) ? b*-1 : b;
					distance = r + g + b;
					if (distance < closest){
						closest = distance;
						tempCluster = clusterCount;
					} else if(distance == 0){
						tempCluster = clusterCount;
						break;
					}
					clusterCount++;
				}
				ListOfClusters[tempCluster]->add(red, green, blue, x, y); 
				clusterSegmentCount[tempCluster] = clusterSegmentCount[tempCluster] + 1;
				red_2 = red_2 + ListOfRGB[tempCluster]->getValueWithID(0, 1);
				green_2 += ListOfRGB[tempCluster]->getValueWithID(0, 2);
				blue_2 += ListOfRGB[tempCluster]->getValueWithID(0, 3);

				if (iterations == 1){
					red = (int)this->getRed(list.getValueWithID(tempCluster, 1), list.getValueWithID(tempCluster, 2));
					green = (int)this->getGreen(list.getValueWithID(tempCluster, 1), list.getValueWithID(tempCluster, 2));
					blue = (int)this->getBlue(list.getValueWithID(tempCluster, 1), list.getValueWithID(tempCluster, 2));
					ListOfRGB[tempCluster]->setValueWithID(0, red, 1);
					ListOfRGB[tempCluster]->setValueWithID(0, green, 2);
					ListOfRGB[tempCluster]->setValueWithID(0, blue, 3);
				}
			
				red_2 = 0;
				green_2 = 0;
				blue_2 = 0;
			// assign each pixel(from image data) to the nearest mean
			}
			
		}
		//cout << clusterSegmentCount[0] << " 1 " << clusterSegmentCount[1] <<  " 2 " << clusterSegmentCount[2] << endl;
		cout << "ITERATION COMPLETE" << endl;
		clusterCount = list.count();
		bool unchanged = true;
		for (int j = 0; j < clusters; j++){
			oldRed = ListOfRGB[j]->getValueWithID(0, 1);
			oldGreen = ListOfRGB[j]->getValueWithID(0, 2);
			oldBlue = ListOfRGB[j]->getValueWithID(0, 3);
			x_cord = ListOfClusters[j]->getValueWithID(0, 4);
			y_cord = ListOfClusters[j]->getValueWithID(0, 5);


			REDCount = ListOfClusters[j]->getAverageWithID(1);
			GREENCount = ListOfClusters[j]->getAverageWithID(2);
			BLUECount = ListOfClusters[j]->getAverageWithID(3);


			if (oldRed != REDCount || oldGreen != GREENCount || oldBlue != BLUECount){
				unchanged = false;
			}
			cout << "Cluster " << j << " NEW R " << REDCount << " G " << GREENCount << " B " << BLUECount << "\nCluster " << j << " OLD R " << oldRed << " G " << oldGreen << " B " << oldBlue << endl;
		
			ListOfClusters[j]->setValueWithID(0, REDCount, 1);
			ListOfClusters[j]->setValueWithID(0, GREENCount, 2);
			ListOfClusters[j]->setValueWithID(0, BLUECount, 3);
			ListOfClusters[j]->setValueWithID(0, x_cord, 4);
			ListOfClusters[j]->setValueWithID(0, y_cord, 5);

			ListOfRGB[j]->setValueWithID(0, REDCount, 1);
			ListOfRGB[j]->setValueWithID(0, GREENCount, 2);
			ListOfRGB[j]->setValueWithID(0, BLUECount, 3);
			cout << "ITERATION segment COMPLETE" << endl;
			// mean of cluster j ← mean of all
			// pixels with label j
		}
		//ListOfRGB[0]->print();
		if (unchanged == true || iterations == MAX_iterations){
			cout << "Clusters are the same or Max iterations was reached.\nIterations went through:  " << iterations << " of MAX " << MAX_iterations << endl;
			stop = 1;
			continue;
		}
		else{
	//		cout << "test" << endl;
			REDCount = 0;
			GREENCount = 0;
			BLUECount = 0;
			for (int j = 0; j < clusters; j++){
				clusterSegmentCount[j] = 0;
				REDCount = ListOfClusters[j]->getValueWithID(0, 1);
				GREENCount = ListOfClusters[j]->getValueWithID(0, 2);
				BLUECount = ListOfClusters[j]->getValueWithID(0, 3);
				x_cord = ListOfClusters[j]->getValueWithID(0, 4);
				y_cord = ListOfClusters[j]->getValueWithID(0, 5);
				

				int LOC = ListOfClusters[j]->count();
				if (LOC == 0){
					ListOfClusters[j]->setValueWithID(0, REDCount, 1);
					ListOfClusters[j]->setValueWithID(0, GREENCount, 2);
					ListOfClusters[j]->setValueWithID(0, BLUECount, 3);
					ListOfClusters[j]->setValueWithID(0, x_cord, 4);
					ListOfClusters[j]->setValueWithID(0, y_cord, 5);
				}
				else{
					int stepper = 0;
					while (stepper < LOC){
						ListOfClusters[j]->removeFirst();
						stepper++;
					}
					ListOfClusters[j]->add(REDCount, GREENCount, BLUECount, x_cord, y_cord);
				}
		//		cout << "CLUSTERS " << j << "  " << clusterSegmentCount[j] << endl;
		//		ListOfClusters[j]->print();
			}
			
		}
	}
	

	//	final: change all pixel's colors to match color of the cluster mean

	cout << "Converting the clusters to pixels. \n This may take awhile." << endl;
	clusterCount = 0;
	int stepper = 0;
	while (clusterCount < list.count()){
		if (clusterCount % 1 == 0){
			cout << "Convertion: " << (((double)clusterCount / (double)list.count()) * (double)100) << "%" << endl;
		}
		stepper = 0;

		Pixel MEANS;
		MEANS.RED = ListOfClusters[clusterCount]->getValueWithID(0, 1);
		MEANS.GREEN = ListOfClusters[clusterCount]->getValueWithID(0, 2);
		MEANS.BLUE = ListOfClusters[clusterCount]->getValueWithID(0, 3);

		int LOC = ListOfClusters[clusterCount]->count();
		while (stepper < LOC){
			if (stepper % (LOC / 10) == 0){
				cout << "Cluster " << clusterCount << " - " << (((double)stepper / (double)LOC) * (double)100) << "%" << endl;
			}
			x_cord = ListOfClusters[clusterCount]->getValueWithID(0, 4);
			y_cord = ListOfClusters[clusterCount]->getValueWithID(0, 5);

			this->setPixel(x_cord, y_cord, MEANS);
			ListOfClusters[clusterCount]->removeFirst();
			stepper++;
		}
		clusterCount++;
	}

	cout << "Clustering completed" << endl;
}