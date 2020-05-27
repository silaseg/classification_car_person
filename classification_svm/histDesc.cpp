
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;


// fuction writeTabToFile() -> une fonction qui enregistre les données d'un tableau dans un fichier
/*void writeTabToFile(float tab[], int size, const char* filename)
{

    printf("%s\n", filename);
    ofstream fout(filename);

    if(!fout)
    {
        cout<<"File Not Opened"<<endl;  return;
    }
    
    for(int i=0; i<size; i++)
    {
	if(tab[i]!=0)
	{
	fout<<i;
	fout<<":";
        fout<<tab[i];
	fout<<" ";
	}
	if(i != size-1)
        	fout<<endl;
    }


    fout.close();
}*/

void writeTabToFile(float tab[], int size, const char* filename)
{

    printf("%s\n", filename);
    FILE *f=fopen(filename,"w");


    if(!f)
    {
        printf("File Not Opened\n");
	return;
    }
    
    for(int i=0; i<size; i++)
    {
	if(tab[i]!=0)
	  fprintf(f,"%d:%f ",(i+1),tab[i]);
    }


  fclose(f);
}
// function main()
int main(int argc, char** argv){
	
	//results output (des fichiers *.txt)
	const char *finalResult="";
	
	const char *imageName;
	int MAX_BINS = 4;

	if (argc >= 1){
		imageName = argv[1];
		if (argc >= 2){
			finalResult = argv[2];
			if (argc >= 3){
				MAX_BINS = atoi(argv[3]);
			}
	  	}
	}
	else
		return -1;

	//image input
    	Mat image = imread(imageName);
	if( image.empty() )
	{ 	
		printf("Error reading image");
		return -1; 
	}

	// message à afficher pendant l'extraction de l'histogramme
	printf("Extraction du descripteur \"histogramme de couleur\" pour %s , %d bins\n", imageName, MAX_BINS);

	int SIZE_BINS = 256/MAX_BINS;//taille de chaque bin
	int FINAL_HIST_SIZE = MAX_BINS*MAX_BINS*MAX_BINS;//taille de l'histogramme

	// l'histogramme final
        float HistFinal[FINAL_HIST_SIZE];
        memset(HistFinal, 0,FINAL_HIST_SIZE*sizeof(float));
	//float HistFinal[FINAL_HIST_SIZE]= {0};

	//des valeurs utilisées pour avoir un indice de l'histogramme final
	int R_MUL=FINAL_HIST_SIZE/MAX_BINS;//16
	int G_MUL=R_MUL/MAX_BINS;//4
	int B_MUL=G_MUL/MAX_BINS;//1

	//les indices de chaque canal de couleurs RGB dans Vec3b d'un pixel
	const int RED_CHAN = 0;
	const int GREEN_CHAN = 1;
	const int BLUE_CHAN = 2;

	//calcul des histogrammes
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
	  	{
	            Vec3b intensity = image.at<Vec3b>(Point(j, i));//Vec3b pour prendre les valeurs R, G et B d'un pixel de l'image

	            int Red = intensity.val[RED_CHAN]/SIZE_BINS;// pour trouver le bin
		    int Green = intensity.val[GREEN_CHAN]/SIZE_BINS;
		    int Blue = intensity.val[BLUE_CHAN]/SIZE_BINS;
		
		    HistFinal[Red*R_MUL + Green*G_MUL + Blue*B_MUL] = HistFinal[Red*R_MUL + Green*G_MUL + Blue*B_MUL] + 1;
	        }

	//calculer N*M pour faire la normalisation
	int im_size = image.rows*image.cols;
	// normalisation
	for(int i = 0; i < FINAL_HIST_SIZE; i++)
	{
		HistFinal[i] = HistFinal[i]/im_size;
	}
	

	//remplir les résultats de l'histogramme dans un fichier *.txt
	writeTabToFile(HistFinal, FINAL_HIST_SIZE, finalResult);

	return 0;
}
