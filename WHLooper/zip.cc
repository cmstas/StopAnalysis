#include <iostream>
#include <string>
#include "TChain.h"
#include "TString.h"
#include "TFile.h"

using namespace std;

//branch types
//0: int
//1: float
//2: vector<int>
//3: vector<float>
void zip(TString slimFilename, TString whFilename){
	TChain * t_wh = new TChain("t_wh");
	t_wh->Add(whFilename);

	TFile slimFile(slimFilename, "UPDATE");
	TTree *slimTree = (TTree*) slimFile.Get("t");

	//Confirm same nentries in both trees
	int nentries_slim = slimTree->GetEntries();
	int nentries_WH = t_wh->GetEntries();

	cout<<"Number of entries slimmed stop tree "<<nentries_slim<<endl;
	cout<<"Number of entries WH tree"<<nentries_WH<<endl;
	if(nentries_slim!=nentries_WH) {
		cout<<"Number of entries do not match."<<endl;
		return;
	}

	//Get list of branches and their types in WH file
	vector<TString> branches_to_add;
	vector<int> branch_types; 
	for(int i=0;i < t_wh->GetListOfBranches()->GetEntries();i++){
		TString this_branch_name = t_wh->GetListOfBranches()->At(i)->GetName();
		int this_type = -1;
		branches_to_add.push_back(this_branch_name);
		//See if it is a vector branch
		TString className = ((TBranch *)t_wh->GetListOfBranches()->At(i))->GetClassName();
		if(className.Contains("vector")){
			if(className.Contains("float")) this_type = 3;
			else this_type=2;
		}
		//if it is not vector, assume is float, unless name begins with "n" (e.g. njets)
		else if (this_branch_name.BeginsWith("n")) this_type=0;
		else this_type = 1;
		branch_types.push_back(this_type);
	}

	vector<int *> int_values;
	vector<float*> float_values;
	vector<vector<int> * > vint_values;
	vector<vector<float> * > vfloat_values;


	//initialize holders for vectors to avoid memory confusion
	
	for(int i =0; i<branches_to_add.size();i++){
		if (branch_types[i]==2){
			vector<int> * this_vint = new vector<int>();
			vint_values.push_back(this_vint);
		}

		if (branch_types[i]==3){
			vector<float> * this_vfloat = new vector<float>();
			vfloat_values.push_back(this_vfloat);
		}
	}

	vector<TBranch *> out_branches;
	vector<TBranch *> in_branches;
	int vicounter=0;
	int vfcounter=0;
	for(int i =0; i<branches_to_add.size();i++){
		//define holders
		//create branches
		cout<<"Adding branch "<<branches_to_add[i]<<", type "<<branch_types[i]<<endl;
//		if(type == 0)
		if (branch_types[i]==0){
			int * this_int = new int();

			TBranch * in_branch = t_wh->GetBranch(branches_to_add[i]);
			in_branch->SetAddress(this_int);

			TBranch * out_branch = slimTree->Branch(branches_to_add[i],this_int,branches_to_add[i]+"/I");
			slimTree->SetBranchAddress(branches_to_add[i],this_int,&out_branch);

			int_values.push_back(this_int);
			in_branches.push_back(in_branch);
			out_branches.push_back(out_branch);
		}	
		else if (branch_types[i]==1){
			float * this_float = new float();

			TBranch * in_branch = t_wh->GetBranch(branches_to_add[i]);
			in_branch->SetAddress(this_float);

			TBranch * out_branch = slimTree->Branch(branches_to_add[i],this_float,branches_to_add[i]+"/F");
			slimTree->SetBranchAddress(branches_to_add[i],this_float,&out_branch);

			float_values.push_back(this_float);
			in_branches.push_back(in_branch);
			out_branches.push_back(out_branch);
		}	
		else if (branch_types[i]==2){
			// vector<int> * this_vint = new vector<int>();

			TBranch * in_branch = t_wh->GetBranch(branches_to_add[i]);
			in_branch->SetAddress(&(vint_values[vicounter]));

			TBranch * out_branch = slimTree->Branch(branches_to_add[i],&vint_values[vicounter]);
			slimTree->SetBranchAddress(branches_to_add[i],&vint_values[vicounter],&out_branch);

			// vint_values.push_back(this_vint);
			in_branches.push_back(in_branch);
			out_branches.push_back(out_branch);
			vicounter++;
		}	
		else if (branch_types[i]==3){
			//vector<float> * this_vfloat = new vector<float>();
			//vfloat_values.push_back(this_vfloat);
			
			TBranch * in_branch = t_wh->GetBranch(branches_to_add[i]);
			in_branch->SetAddress(&(vfloat_values[vfcounter]));
			// TBranch * in_branch = 0;
			// t_wh->SetBranchAddress(branches_to_add[i],&vfloat_values[vfcounter],&in_branch);
			
			
			TBranch * out_branch = slimTree->Branch(branches_to_add[i],&(vfloat_values[vfcounter]));
			slimTree->SetBranchAddress(branches_to_add[i],&(vfloat_values[vfcounter]),&out_branch);

			in_branches.push_back(in_branch);
			out_branches.push_back(out_branch);
			vfcounter++;
			
			
		}	

	}

	cout<<"Number of input branches "<<in_branches.size()<<endl;
	cout<<"Number of output branches "<<out_branches.size()<<endl;

	//Event loop
	for(int i =0;i<t_wh->GetEntries();i++){
		//cout<<"Event "<<i<<endl;
		t_wh->GetEntry(i);
		// Long64_t tentry = t_wh->LoadTree(i);
		// for(int j=0;j<in_branches.size();j++){in_branches[j]->GetEntry(tentry);}
		// cout<<*(int_values[0])<<endl;
		//cout<<vfloat_values.size()<<endl;
		//for(int j=0;j< vint_values.size();j++) { cout<<vint_values[j]->size()<<endl; if(vint_values[j]->size()>0) cout<<vint_values[j]->at(0)<<endl;}

		//for(int j=0;j< 4;j++) { cout<<vfloat_values[j]->size()<<endl; if(vfloat_values[j]->size()>0) cout<<vfloat_values[j]->at(0)<<endl;}
		//cout<<vfloat_values[0]->at(0)<<endl;
		for(int j=0;j<out_branches.size();j++){ out_branches[j]->Fill();}
	}
	slimTree->Write();
	slimFile.Close();
}

/*int main(int argc, char** argv)
{
	zip(argv[1],argv[2]);
}*/