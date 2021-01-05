#include "version.h"
#include "matrix.h"
#include "table_format.h"
//#include <ctime>

void printhelp(){
	cout << "MS-get-pc version :" << version << endl;
	cout << "\tCompute the k-dimension principle coordinates by PCoA" << endl;
	cout << "Usage: " << endl;
	cout << "MS-get-pc [Option] Value" << endl;
	cout << "Options:" << endl;
	cout << "[Input options, required]" << endl;
	cout << "\t-d Input distance file" << endl;
	cout << "\t-k the number of dimension principle coordinates, default is 3" << endl;
	cout << "[Output options]" << endl;
	cout << "\t-o Output test samples pc file, default is \"test.pc\"" << endl;
	cout << "[Other options]" << endl;
	cout << "\t-h Help" << endl;
}

string dist_file;
string out_pc_file = "test.pc";
string K = "3";

int Parse_Para(int argc, char * argv[]){
	int i = 1;
	if (argc ==1) printhelp();
	while(i<argc){
		if (argv[i][0] != '-') {
			cerr << "Argument # " << i << " Error : Arguments must start with -" << endl;
			exit(0);
		};
		switch(argv[i][1]){
			case 'd': dist_file = argv[i+1]; break;
			case 'k': K = argv[i+1]; break;
			case 'o': out_pc_file = argv[i+1]; break;
			case 'h': printhelp(); break;
			default :
				cerr << "Error: Unrec argument " << argv[i] << endl;
				printhelp();
				break;
		}
		i += 2;
	}
	return 1;
}

vector<string> samples;
vector<vector<float> > Dis;

int main(int argc, char *argv[]) {
	
	Parse_Para(argc, argv);
	Matrixx dis(dist_file.c_str());
	samples = dis.Get_Sample_Names();
	int k = atoi(K.c_str());

	//time_t a = time(0);

	float ** M_pc = dis.Get_PC_Matrix(k);
	float * percentage = dis.get_percentage(k);	
	
	//time_t b = time(0);
	//cout << "time:" << endl << b - a << endl;

	Output_Table(samples, M_pc, out_pc_file.c_str(), k);

	for(int i = 0; i < k; i++) {
		cout << percentage[i] << "\t";
	}
	cout << endl;

	return 0;
}
