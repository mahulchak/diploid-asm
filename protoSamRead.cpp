#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<algorithm>

using namespace std;
struct qord {
	string chrom;
	int pos;
	char ref;//character state
	char alt;
	};
vector<string> splitField(string & str, char c);
void readFasta(ifstream & fasm,map<string,string> & refAsm);
int main()
{
	ifstream fsam,fvcf,fasm;
	vector<string> vs;
	string samLine, cigar,str, qSeq,vcfLine ,refChrom,rName;
	int pos = 0, pos1 =0, refPos =0, qPos =0;
	map<int,int> aln;//stores base to base mapping of an alignment
	map<string,string> refAsm;
	vector<qord> snpPos;//stores SNP positions
	qord tempQord;
	fvcf.open("clean.recode.vcf");//open the vcf file to read the SNPs
//	while(getline(fvcf,vcfLine))
//	{
//		if(vcfLine[0] != '#')
//		{
//			vs = splitField(vcfLine,'\t');
//			if(vs[3].size() == 1)
//			{
//				tempQord.chrom = vs[0];
//				tempQord.pos = stoi(vs[1]);
//				tempQord.ref = vs[3][0];//assign the state
//				tempQord.alt = vs[4][0];
//				snpPos.push_back(tempQord);
//				//cout<<vcfLine<<endl;
//				//cout<<tempQord.chrom<<'\t'<<tempQord.pos<<'\t'<<tempQord.ref<<'\t'<<tempQord.alt<<endl;
//			}
//		}	
//	}		
	fvcf.close();
	fasm.open("iso1.scaffold.fasta");//open the genome
	readFasta(fasm,refAsm);//read the genome assembly into refAsm
	fasm.close();
	fsam.open("hyb_iso1-ref.sorted.sam");//get the read alignments
	while(getline(fsam,samLine))
	{
		if(samLine[0] != '#')
		{
			vs = splitField(samLine,'\t');
			cigar = vs[5];
			rName = vs[0];//read name
			refPos = stoi(vs[3]);
			refChrom = vs[2];
			qSeq = vs[9];
			cout<<cigar<<endl;
			pos1 = -1;
			qPos = 0;//reset qPos
			for(unsigned int i=0; i<cigar.size();i++)
			{
				if(cigar[i] == 'M')
				{
					pos = i;
					str = cigar.substr(pos1+1,pos-pos1-1);
					pos1 = pos;
					cout<<str<<endl;
					for(int i=0;i<stoi(str);i++)
					{
						refPos++;
						qPos++;
cout<<refChrom<<'\t'<<refPos<<'\t'<<refAsm[refChrom][refPos-1]<<'\t'<<rName<<'\t'<<qPos<<'\t'<<qSeq[qPos]<<endl;
					}						
					//refPos = refPos + stoi(str);//both reference and query increases
					//qPos = qPos + stoi(str);
				}
				if(cigar[i] == 'S')
				{
					pos = i;
					str = cigar.substr(pos1+1,pos-pos1-1);
					pos1 = pos;
					cout<<str<<endl;
					qPos = qPos + stoi(str);		
				}
				if(cigar[i] == 'D')
				{
					pos = i;
					str = cigar.substr(pos1+1,pos-pos1-1);
					pos1 = pos;
					cout<<str<<endl;
					refPos = refPos + stoi(str);//only reference increases
				}
				if(cigar[i] == 'I')
				{
					pos = i;
					str = cigar.substr(pos1+1,pos-pos1-1);
					pos1 = pos;
					cout<<str<<endl;
					qPos = qPos + stoi(str);//only query increases
				}
				if(cigar[i] == 'H')
				{
					pos = i;
					str = cigar.substr(pos1+1,pos-pos1-1);
					pos1 = pos;
					cout<<str<<endl;
					qPos = qPos + stoi(str);//only query increases
				}
				if(cigar[i] == 'N')
				{
					pos = i;
					str = cigar.substr(pos1+1,pos-pos1-1);
					pos1 = pos;
					cout<<str<<endl;
					refPos = refPos + stoi(str);
				}
				if((cigar[i] == '=') ||(cigar[i] == 'X'))
				{
					pos = i;
					str = cigar.substr(pos1+1,pos-pos1-1);
					pos1 = pos;
					cout<<str<<endl;
					refPos = refPos + stoi(str);
					qPos = qPos + stoi(str);	
				}
			}
			pos = 0;//reset the positions
			pos1=0;
	//		if(vs[3].size() == 1)
	//		{
	//			cout<<samLine<<endl;
	//		}
		}
			
	}
	fsam.close();	
	return 0;
}



//////////////////////////////////////////////////////
vector<string> splitField(string & str, char c)
{
	size_t pos =1, pos1 =0;
	vector<string> vs;
	string tempstr;
//need to add a way to add the last element
	while(pos1 <str.size())
	{
		pos1 = str.find(c,pos);
		if(pos1 < str.size())
		{
			tempstr = str.substr(pos-1,pos1-pos+1);
			if(tempstr[0] == '\t')
			{
				tempstr = tempstr.substr(1); //remove the preceding delimiter
			}
			pos = pos1+1;
			vs.push_back(tempstr);
			//cout<<tempstr<<endl;
		}
	}
	tempstr = str.substr(pos);
	vs.push_back(tempstr);
	return vs;
}
///////////////////////////////////////////////////////
void readFasta(ifstream & fasm,map<string,string> & refAsm)
{
	string str,name;
	while(getline(fasm,str))
	{
		if(str[0]== '>')
		{
			name = str.substr(1);
		}
		if(str[0] != '>')
		{
			refAsm[name].append(str);
		}
	}
}
