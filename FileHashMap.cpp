#include "FileHashMap.h"
#include <vector> 
#include <functional>
#include <string> 
#include <random>
#include <unordered_map>
using std::cout;
using std::vector;
using std::hash;
using std::string;
using std::unordered_map;

void testone(){

	hash<char> hash_fn;
	FileHashMap<char,int,hash<char>> fmap("fmap.txt",hash_fn);
	FileHashMap<char,int,hash<char>> fmap_CP("fmap_CP.txt",hash_fn);

	std::pair<char,int> st;
	st = std::make_pair('a',22);
	fmap.insert(st);
	
	std::pair<char,int> tt;
	tt = std::make_pair('c',23);

	std::pair<char,int> ut;
	ut = std::make_pair('r',100);

	std::pair<char,int> vt;
	vt = std::make_pair('p',49);


	std::pair<char,int> ovrwrt;
	ovrwrt = std::make_pair('r',42);

	auto x = fmap.insert(tt);
	auto z = fmap.insert(tt); 

	fmap_CP.insert(st);
	fmap_CP.insert(tt);

	if(x.first == z.first){
		cout<< "Insert/Iterator worked\n";
	}

	if(fmap == fmap_CP){
		cout << "EQUALITY WORKED\n";
	}
	if(fmap != fmap_CP){
		cout << "EQUALITY FAILED\n";
	} else cout << "EQUALITY WORKED\n";

	auto fnd = fmap.find(tt.first);
	if(fnd == x.first){
		cout << "Find worked \n";
	}

	auto notfnd = fmap.find('X');
	if(notfnd != fmap.end()){
		cout << "notfnd Failed\n ";
	}


	auto mpd = fmap['a'];
	if(mpd == st.second){
		cout << "Bracket Worked\n"; 
	}

	auto tht = fmap.set(ut);
	cout << fmap['r'] << "\n";
	

	cout << (*wrtn).first << " " << (*wrtn).second;
	auto wrtn = fmap.set(ovrwrt);
	cout << fmap['r'] << "\n";

	if(fmap.count('r') != 1){
		cout << "Count failed \n";
	}
	if(fmap.count('D') != 0){
		cout << "count non-exist failed \n";
	}

	fmap.clear();
	if(fmap.size() != 0 && fmap.empty()){
		cout << "clear failed\n";
	}
	fmap.insert(ut);
	fmap.insert(tt);
}

void testtwo(){
	hash<char> hash_fn;
	FileHashMap<char,int,hash<char>> scdmap("Scd.txt",hash_fn);
	FileHashMap<char,int,hash<char>> scdmap_CP("Scd_CP.txt",hash_fn);

	int incr = 32; 
	char incrchar = 'A';
	std::pair<char,int> st;
	st = std::make_pair(incrchar,incr);
	auto tstr = scdmap.insert(st); 
	for(int i = 0;i<200;++i){
		++incr; 
		
		++incrchar;
		if(incrchar >=254){incrchar = 'A';}
		st = std::make_pair(incrchar,incr);
		tstr = scdmap.insert(st);
	}
	std::pair<char,int> ovr;
	ovr = std::make_pair('A',300);
	scdmap.set(ovr);
/*
	scdmap.clear();
	
	scdmap.set(st);
	incr = 32; 
	incrchar = 'A';
	for(int i = 0;i<20;++i){
		++incr; 
		++incrchar;
		st = std::make_pair(incrchar,incr);
		auto nwtst = scdmap.set(st);
		if((*nwtst).first != (*(scdmap.find(st.first))).first){
			cout << "set after clear: " << i << "failed\n";
		} 
	}
		scdmap.insert(std::make_pair('q',22));
		if(scdmap['q'] != 22){
			cout << "insert after clear failed\n"; 
		}
		scdmap.insert(std::make_pair('w',01));
		if(scdmap['w'] != 01){
			cout << "insert after clear failed\n";
		}

*/
}

void testthree(){
	std::hash<int> inthsh;
	FileHashMap<int,int,hash<int>> thrdmp("thrd.txt",inthsh);
	unordered_map<int,int> mp;
	
	int frst = 50;
	std::pair<int,int> st;
	auto thngt = std::make_pair(frst,-1);
	auto tstr = thrdmp.insert(thngt); 
	for(int i = 0;i<772;++i){
		if (i%2==0){
			frst++;
		} else {
			frst += 2;
		}
		thngt = std::make_pair(frst,i);
		thrdmp.set(thngt);
		mp.insert(thngt);
		if(thrdmp[mp.begin() -> first]!= mp.begin() -> second){
			cout << "Value is incorrect on set or insert\n";
		} 
	}
	for(int i = 0;i<772;++i){
		if(thrdmp[mp.begin() -> first]!= mp.begin() -> second){
			cout << "Equality Check Failed\n";
		} 
	}

	//This is what breaks it
	thrdmp.clear();
	cout << "clearing on thrdmap: ";
	mp.clear();

	for(int i = 0;i<772;++i){
		if (i%2==0){
			frst++;
		} else {
			frst += 2;
		}
		thngt = std::make_pair(frst,i);
		thrdmp.set(thngt);
		mp.insert(thngt);
		if(thrdmp[mp.begin() -> first]!= mp.begin() -> second){
			cout << "Value is incorrect on set or insert (after clear) \n";
		} 
	}
	for(int i = 0;i<772;++i){
		if(thrdmp[mp.begin() -> first]!= mp.begin() -> second){
			cout << "Equality Check after clear failed\n";
		} 
	}



}




int main (int argc, char const *argv[]){
	testone();
	testtwo();
	//OrderedTest();
	testthree();
	return 0;
}