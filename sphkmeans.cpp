#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <cassert>
#include <math.h>
#include <stdio.h>
#include <map>
#include <cstdio>
#include <time.h>
#include <iomanip>

using namespace std;


int n_clusters;
int number_of_trials;
string input_file;
string output_file;
string class_file;

int ob_ptr=0;
int dim_ptr=0;
int count_changes=0;
int max_dim=0;
float objective_trial_n=0;
float best_objective=0;
int best_trial=0;

std::map<int, vector<int> > clustering_solution;

vector<vector<int> > distribution_matrix; //Hard-Coding to hold distribution of objects across clusters
vector<float> entropy ;
vector<float> purity ;


int start=0, stop=0;

//To use in clusterize fn

int num_of_its=0;
std::vector<int> object_info;
std::map<int, vector<int> > cluster_info;

float **centroid;

std::vector<int> class_info;


void read_classfile()
{
		string ID1, ID2;
		std::map<string, int> mapping1;

		int count = 0;
		ifstream fc;
		fc.open(class_file.c_str());
		while(fc)
		{
				count++;

				std::getline(fc, ID1, ',');
				std::getline(fc, ID2);
				//cout<<ID1<< " " << ID2<<endl;
				if ( mapping1.find(ID2) == mapping1.end() ) {
						mapping1[ID2]= mapping1.size();
				}

				class_info.push_back(mapping1[ID2]);

		}

		class_info.pop_back();


}



void writeToFile(){

		FILE* fp = fopen(output_file.c_str(),"w");
		if(!fp) return;
		int j=0;
		for (int i=0; i<clustering_solution.size(); i++)
		{
				j=0;
				while(j!= clustering_solution[i].size())
				{
					fprintf(fp,"%d,%d\n",clustering_solution[i][j],i);
					j++;
				}
		}
		fclose(fp);
}

void cal_entropy()
{
		int class_num= -1;
		float prob=0;
		float max_prob =0;
		float final_entropy=0;
		float final_purity=0;
		vector<vector<int> > init_to_zero(n_clusters, vector<int>(20,0)); //Initializing a dummy variable with k X 20 to hold class distribution within the clusters
		distribution_matrix= init_to_zero;

		vector<float> init_to_zero2(n_clusters, 0); //Initializing a dummy variable with k X 20 to hold class distribution within the clusters
		entropy= init_to_zero2;
		purity= init_to_zero2;

		int total=0;
		//Getting the distribution
		for (int k=0; k<clustering_solution.size();k++)
		{

				for (int j=0; j< clustering_solution[k].size(); j++)
				{
						class_num= class_info[clustering_solution[k][j]];
						distribution_matrix[k][class_num-1]++;


				}
		}

		//cout<<"total=" <<total;


		for (int k=0; k < distribution_matrix.size();k++)
		{
				for (int i=0; i< 20; i++)
				{
						cout<< setw(3)<< distribution_matrix[k][i]<< " ";
						total+=distribution_matrix[k][i];

				}

				cout<<endl;
		}




		cout<<endl<<endl<<"Entropy and Purity:"<<endl;

		for (int k=0; k < distribution_matrix.size(); k++)
		{
				max_prob=0;
				for (int i=0; i< 20; i++)
				{
						prob= (float)distribution_matrix[k][i]/(float)clustering_solution[k].size();

						if(prob!=0)
								entropy[k]+= prob*(log2(prob));

						if (prob>max_prob)
								max_prob = prob;
				}

				purity[k]= max_prob;
				cout<<"entropy="<<entropy[k]<<" "<<"Purity="<<purity[k] <<endl;
		}

		//cout<<"total entropy:"<<endl;

		for(int e=0; e< entropy.size(); e++)
		{
				final_entropy+=  ((float)(clustering_solution[e].size()/(float)total) * (float) entropy[e]);
				final_purity += ((float)(clustering_solution[e].size()/(float)total) * (float) purity[e]);
		}

		cout<<"Total entropy:"<<final_entropy << endl;
		cout<<"Total purity:"<<final_purity;

}



//Generate k random centroids with seed passed as parameter and return an array
void random_k_generator(int seed, int*temp)
{
		//int a[n_clusters];
		srand(seed);
		for(int n=0; n<n_clusters; n++)
		{
				temp[n]=rand() % (ob_ptr+1) + 0;

		}

		//return a;
}

void centroid_initializer(int *ob, int *dim, float *val, int *temp)
{

		centroid = (float **)calloc(n_clusters+1,sizeof(float*));
		for(int i=0;i<n_clusters;i++)
				centroid[i] = (float*)calloc(max_dim+1,sizeof(float));


		int j=0, beg=0, end1=0;

		for(int i=0; i<n_clusters;i++)
		{
				beg= (temp[i]==0 ? 0 : ob[temp[i]-1]+ 1);
				//cout<<"beg= "<<beg<<endl;
				end1= ob[temp[i]];
				j=beg;
				while(j<=end1)
				{
						centroid[i][dim[j]-1]= val[j];
						//cout<<dim[j]-1<<endl;
						j++;
				}
		}

}

//Normalize the values in the kth centroid
void centroid_normalizer ()
{
		float mod[n_clusters];

		//float modi=0;
		for(int k=0;k<n_clusters;k++)
		{
				mod[k]=0;
				for (int j=0; j<max_dim;j++)
				{
						mod[k]+= pow(centroid[k][j],2);
				}

				mod[k]= pow(mod[k], 0.5);
		}


		for(int k=0;k<n_clusters;k++)
		{

				for(int j=0;j<max_dim;j++)
				{
						centroid[k][j]= centroid[k][j]/mod[k];
				}

		}
}

void centroid_updater(int *ob, int *dim, float *val)
{

		for(int i=0;i<n_clusters;i++)
		{
				for (int j=0; j<max_dim; j++)
						centroid[i][j]=0;
		}


		int j=0, beg=0, end1=0, temp=0, size_of_cluster_k=0;

		for(int k=0;k<cluster_info.size();k++)
		{
				size_of_cluster_k=cluster_info[k].size();
				for(int i=0; i<size_of_cluster_k;i++)
				{
						temp= cluster_info[k][i];
						beg= (temp==0 ? 0 : ob[temp -1]+ 1);

						end1= ob[temp];
						j=beg;
						while(j<=end1)
						{
								centroid[k][dim[j]-1]+= (val[j]/size_of_cluster_k);

								j++;
						}
				}




		}
}


void print_object_info()
{
		for (int i=0;i!= object_info.size();i++)
		{
				cout<<i<< " "<<object_info[i]<<endl;
		}
}

void clusterize(int *ob, int* dim, float *val)
{
		int i=0,j=0,k=0;
		int beg=0, end1=0, c_i=0;
		float cos_i_k=0, cos_i=0;
		objective_trial_n=0;

		//Initialize cluster_info to empty map
		cluster_info.clear();

		for (i=0;i<=ob_ptr;i++)
		{
				beg=(i==0? 0: ob[i-1]+1);
				end1= ob[i];
				cos_i=0;
				int kk=0;
				for(k=0;k< n_clusters;k++)
				{
						j=beg;
						cos_i_k=0;
						while (j<= end1)
						{
								cos_i_k += val[j]* (centroid[k][dim[j]-1]);
								j++;
						}

						if(cos_i_k > cos_i)
						{
								cos_i=cos_i_k;
								c_i= k;
						}
				}

				if(object_info[i]!= c_i)
				{
						count_changes++;

				}


				objective_trial_n+=cos_i;
				object_info[i]=c_i;
				cluster_info[c_i].push_back(i);
		}
}



int main(int c, char** argv)
{

		input_file=argv[1];
		class_file=argv[2];
		n_clusters= atoi(argv[3]);
		number_of_trials= atoi(argv[4]);
		output_file= argv[5];
		read_classfile();
		string ID;
		vector<int> o;
		vector<int> dm;
		vector<float> vm;
		int count = 0;
		ifstream f1;
		f1.open(input_file.c_str());
		while(f1)
		{
				count++;

				std::getline(f1, ID, ',');
				o.push_back(atoi(ID.c_str()));
				std::getline(f1, ID, ',');
				dm.push_back(atoi(ID.c_str()));
				std::getline(f1, ID);
				vm.push_back(atoi(ID.c_str()));

		}
		//cout<< count << endl;
		/*   for (int i=0;i<200;i++)
			 {
			 cout<<i+1<<" "<<o[i]<<" "<<d[i]<<" "<<v[i]<<endl;
			 }*/
		//cout<<endl<<"count="<<count;
		cout<<"Total number of non- zero dimensions in the entire data set: "<< --count << endl;
		//The above is because it stores a line extra. So at count-1= total number of lines.
		//It is stored in o from 0 to (count-1)-1 th location = count-2
		//cout<<count;

		int x= o[count-1]; //x is the object stored in the last line
		//cout<<"x="<<x;
		int ob[x];

		int *d = (int*)calloc(count+1,sizeof(int));
		float *v = (float*)calloc(count+1,sizeof(float));

		int prev= o[0];
		int cur= prev;
		int i;


		for(i=0; i<=count; i++) //o.size = total no if lines including one extra blank line
		{
				cur= o[i];

				d[i]=dm[i];
				//Taking max_dim
				if(d[i]>max_dim)
						max_dim=d[i];

				v[i]=vm[i];

				if(cur!=prev)
				{
						ob[ob_ptr++]= i-1;
						prev=cur;
						//cout<<ob_ptr<<endl;


				}


		}


		dim_ptr= i-2;
		ob[ob_ptr]= dim_ptr; //Same as storing count -2
		//cout<<endl<<"dim_ptr="<<dim_ptr;

		//dim_ptr=i-2;
		//cout<<"dim_ptr="<<dim_ptr<<endl;
		cout<<"Number of Objects= "<<ob_ptr+1<<endl;
		cout<<"Number of distinct dimensions= "<<max_dim <<endl;
		vector<int>().swap(o);



		int j=0;
		float t_mod=0;
		float mod[x];

		//Calculating mod
		for(int i=0;i<=ob_ptr;i++)
		{
				t_mod=0;

				while(j<= ob[i])
				{
						//cout<<j+1<<" "<<i+1<<" "<<d[j]<<" "<<v[j]<<endl;
						t_mod+= pow(v[j],2);
						j++;
				}

				mod[i]= pow(t_mod,0.5);
		}


		//Dividing by mod
		float s=0;
		j=0;
		for(int i=0;i<=ob_ptr;i++)
		{

				while(j<= ob[i])
				{
						v[j]=v[j]/mod[i];

						j++;

				}

		}

		//Initialize cluster_info
		vector<int> init_to_minus1(ob_ptr+1, -1);



		//Find the randomly selected k objects to be taken as the initial centroid
		int temp[n_clusters];


		for(int t=1; t<= number_of_trials; t++)

		{
				cout<<endl<<"Trial: "<< t ;

				object_info= init_to_minus1;

				//selection of k objects randomly using seed (2t -1) and storing in temp
				random_k_generator((2*t)-1, temp);
				// random_k_generator(20, temp);
				//cout<<endl<<"temp[0]"<<temp[0];

				//Initialize the centroid matrix
				centroid_initializer(ob, d, v, temp);



				//The heart!
				do

				{
						//cout<<"Iteration no "<<(++num_of_its);
						count_changes=0;

						//Assign the point to clusters
						clusterize(ob,d,v);


						//Reassign the centroids
						centroid_updater(ob,d,v);


						//Normalize centroids
						centroid_normalizer();


				} while(count_changes > 32);

				//print_object_info();


				if(objective_trial_n > best_objective)
				{
						best_objective = objective_trial_n;
						clustering_solution= cluster_info;
						best_trial= t;
				}

				cout<<" Objective: "<< objective_trial_n << endl;

		}

		cout<<endl<<"Trial with the maximun objective:" << best_trial << " objective value: "<< best_objective<<endl << endl;
		cal_entropy();

		writeToFile();





		return 0;
}



