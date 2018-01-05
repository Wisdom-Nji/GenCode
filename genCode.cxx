#include <fstream>
#include "Class/Form.hpp"

using namespace std;

int main(int argc, char** argv) {
	if(argc != 5) {
		cout << "Error with input... please check all syntax!" << endl;
		return 1;
	}
	string input_file, output_file;

	for(int i=1;i<argc;++i) {
		if(string(argv[i]) == "-f") {
			input_file = argv[i+1];
			++i;
		} else if(string(argv[i]) == "-o") {
			output_file = argv[i+1];
			++i;
		}
	}

	cout << "Input file: " << input_file << "\nOutput file: " << output_file << endl;

	cout << "Running Java parser..." << endl;
	string javaCommand = "java -jar genCode.jar -f " + input_file + " -o Generated/" + output_file + ".gcd";
	system(javaCommand.c_str());

	string gen_filename = output_file + ".gcd";
	ifstream check_file(string("Generated/" + gen_filename).c_str());
	if(check_file.good()) cout << "Output generated!..." << endl;
	else cout << "No output generated!..." << endl;
	check_file.close();


	ifstream readfile(string("Generated/" + gen_filename).c_str());
	ofstream writefile(string("Generated/" + output_file).c_str());
	string temp, conditions = "<?php\n\n";

	vector<Form> forms;
	vector<string> file;

	if(!readfile.good()) {
		cout << "Input file can't be read! FILE[" << input_file << "]" << endl;
		return 2;
	}
	if(!writefile.good()) {
		cout << "Output file can't be read! FILE[" << output_file << "]" << endl;
		return 2;
	}

	while(getline(readfile, temp)) file.push_back(temp);
	readfile.close();
	//system(string("rm Generated/" + gen_filename).c_str());

	Form form;
	for (auto i: file) {
		if(i.find("Start-Form") != string::npos)  {
			form = Form::get();
			continue;
		}
		else if(i.find("End-Form") != string::npos) {
			forms.push_back(form);
			continue;
		}
		else form.add(i);
	}
	cout << "Number of forms: " << forms.size() << endl;
	for(auto i: forms) i.write(conditions);
	cout << "Done writing.." << endl;
	conditions += "\n\n?>\n";
	
	writefile << conditions;
	writefile.close();

	return 0;
}