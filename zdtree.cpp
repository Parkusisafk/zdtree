#include <bits/stdc++.h>
using namespace std;

struct zdtree{
	private:
	vector<long long> realtree;
	vector<long long> rotated_potato;
	long long featuretc;
	long long time = 0;
	long long rows;
	long long rotato;
	long long potato;
	long long rotnum = 0;
	long long inf = 2e17;
	
	void call_exit_routine(string somethingwrong){
		cerr<<somethingwrong<<endl;
		exit(67);
	}
	
	long long ipow(long long base, long long exp){
		long long start = 1;
		while(exp--) start *= base;
		return start;
	}
	
    
	public:
	zdtree(vector<vector<long long>> vin){
		K0_init();
		if((int)(vin.size()) == 0) call_exit_routine("check vector");
		featuretc = (long long)(vin[0].size()) + 1; //defautl is rank + time for size 1
		if(featuretc <= 1) call_exit_routine("not enough arguments in vector");
		init(featuretc);
		for(vector<long long> vlayer : vin){
			if((long long)(vlayer.size()) != featuretc - 1) call_exit_routine("one of the vector array has abnormal size");
			treeadd_singular(vlayer,time++);
		}
	}
	
	zdtree(vector<long long> vin){
		K0_init();
		featuretc = 2;
		init(2);
		if((long long)(vin.size()) == 0) init(2);
		else{
			for(auto& c : vin) treeadd_singular({c},time++);
		}
	}
	
	zdtree(){
		K0_init();
		init(2);
	}
	
	inline void K0_init(){
		random_device rd;
		mt19937 gen(rd());
	    uniform_int_distribution<long long> distrib(1,5);
    	rotato = distrib(gen);
    	potato = distrib(gen);
    	
    	rotated_potato.resize(1000);
    	for(long long x = 1; x < (long long)(rotated_potato.size()); ++x){
    		rotated_potato[x] = rotato + ipow(potato,rotated_potato[x-1] % distrib(gen)) + ipow(rotato,rotated_potato[x-1] % distrib(gen));
    	}
	}
	
	inline void init(long long featcount){
		realtree.resize(1 + 1*featcount + 4*featcount); //0, up 1*2, side 4*2
		featuretc = featcount;
		rows = 1;
		realtree[0] = 1; //set num to 1 (able to hold 1 value with default 2 features)
		
		for(long long x = featuretc*realtree[0]+featuretc+1; x < (long long)(realtree.size()); ++x){
			realtree[x] = inf; //init val
		}
	}
	
	void treeadd_singular(vector<long long> vlayer, long long timein){
		potato_rotato();
		//make space first
		need_one_space();
		
		long long num = realtree[0];
		
		long long timerank = add_item_to_tree(0,timein);
		
		long long baseindex = (num-1)*featuretc + 1;

		realtree[baseindex] = timerank;
		
		for(long long x = 1; x <= (long long)(vlayer.size()); ++x){
			long long featureranktemp = add_item_to_tree(x,vlayer[x-1]);
			realtree[baseindex + x] = featureranktemp;
		}
		//fills in the vertical pole
		
		rotato_potato();	
	}
	
	inline void need_one_space(){
		//pole need featuretc number of new spaces
		long long num = realtree[0];

		long long cursize = (long long)(realtree.size());
		realtree.resize(cursize + featuretc);
		for(long long x = cursize-1; x > num*featuretc; --x){
			realtree[x+featuretc] = realtree[x];
		}
		
		for(long long x = num*featuretc+1; x <= num*featuretc + featuretc; ++x){
			realtree[x] = 0;
		}
		realtree[0]++;
		num++;
		//check if 3^n is less than new num
		
		if(num > ipow(3,rows)){
			//need shift the entire row down by 1, and reset rotato potato
			vector<long long> temp;
			
			
			long long maxrowidx = num*featuretc + featuretc*(ipow(3,rows+1)-1)/2;
			long long minrowidx = num*featuretc + featuretc*(ipow(3,rows)-1)/2; //inaccessible
			
			temp.assign(realtree.begin() + minrowidx + 1, realtree.begin() + maxrowidx + 1);
			
			potato_rotato_v(rows,temp);
			rotnum = 0;
			rows++;
			
			maxrowidx = num*featuretc + featuretc*(ipow(3,rows+1)-1)/2;
			minrowidx = num*featuretc + featuretc*(ipow(3,rows)-1)/2; //inaccessible
			vector<long long> res = {};
			vector<long long> blank(2*ipow(3,rows-1),inf);
			for(long long x = 0; x < featuretc; ++x){
				vector<long long> due;
				due.assign(temp.begin()+x*ipow(3,rows-1),temp.begin()+(x+1)*ipow(3,rows-1));
				
				res.insert(res.end(),due.begin(),due.end());
				res.insert(res.end(),blank.begin(),blank.end());
				
			}
			
			realtree.insert(realtree.end(),res.begin(),res.end());
			
			for(long long x = 0; x < featuretc; ++x){
				refresh_tree(x);
			}
		}
		
	}
	
	long long add_item_to_tree(long long featureindex, long long featurevalue){
		long long num = realtree[0];
		long long curidx = num*featuretc + featureindex + 1;
		curidx = get_childrenstartplace(1,curidx,featureindex);
		
		for(long long x = 1; x < rows; ++x){
			long long optidx = select_correct_index(curidx,featurevalue,x);
			curidx = get_childrenstartplace(x+1,optidx,featureindex);
		}

		long long insidx = select_correct_index_insert(curidx,featurevalue,rows);
		//insert at x means that x becomes that element, pushes the rest forward
		//need add insidx corrected to rotation
		insidx = potato_rotato_s(rows,insidx);
		long long finalised_rank = insidx - num*featuretc - featuretc*(ipow(3,rows)-1)/2 - featureindex*(ipow(3,rows));
		long long final_finalised_rank = num - finalised_rank;
		//firstly update the ranks to +1
		update_ranks_add(featureindex,final_finalised_rank);
		//update current row
		vector<long long> temp;
		long long maxrowfeatidx = num*featuretc + featuretc*(ipow(3,rows)-1)/2 + featureindex*ipow(3,rows) + ipow(3,rows);
		long long minrowfeatidx = num*featuretc + featuretc*(ipow(3,rows)-1)/2 + featureindex*ipow(3,rows); //inaccessible
		long long minrowidx = num*featuretc + featuretc*(ipow(3,rows)-1)/2; //inaccessible
		long long maxrowidx = num*featuretc + featuretc*(ipow(3,rows+1)-1)/2;
		temp.assign(realtree.begin() + minrowidx + 1, realtree.begin() + maxrowidx + 1);
		
		potato_rotato_v(rows,temp);
		
		vector<long long> temp2;
		temp2.assign(temp.begin() + featureindex*ipow(3,rows) + finalised_rank - 1, temp.begin() + featureindex*ipow(3,rows) - 1 + num);
		copy(temp2.begin(),temp2.end(),temp.begin() + finalised_rank + featureindex*ipow(3,rows));
		temp[finalised_rank -1 + featureindex*ipow(3,rows)] = featurevalue;
		
		rotato_potato_v(rows,temp);
		
		copy(temp.begin(),temp.end(),realtree.begin() + minrowidx + 1);
		
		//update all above layers members with new rotato 
		refresh_tree(featureindex);
		//then add the data for this rank
		return final_finalised_rank;
		
		//and then im done?
	}
	
	inline long long select_correct_index(long long curidx, long long featurevalue, long long rownum){
		if(realtree[curidx] == inf) return curidx;
		if(featurevalue <=realtree[curidx]) return curidx;
		if(featurevalue >=realtree[account_for_potato(rownum,curidx+2)]) return account_for_potato(rownum,curidx+2);
		else return account_for_potato(rownum,curidx+1);
	}
	inline long long select_correct_index_insert(long long curidx, long long featurevalue, long long rownum){
		if(realtree[curidx] == inf) return curidx;
		if(featurevalue <=realtree[curidx]) return curidx;
		if(featurevalue <=realtree[account_for_potato(rownum,curidx+1)]) return account_for_potato(rownum,curidx+1);
		if(featurevalue <=realtree[account_for_potato(rownum,curidx+2)]) return account_for_potato(rownum,curidx+2);
		else return account_for_potato(rownum,curidx+3);
	}
	
	inline long long get_childrenstartplace(long long rownum, long long optidx, long long featureindex){
		//do math here with potato rotato
		long long lastlayer = optidx - realtree[0]*featuretc - featuretc*(ipow(3,rownum-1)-1)/2 - 1;
		long long newlayerstart = realtree[0]*featuretc + featuretc*(ipow(3,rownum)-1)/2; //inaccessible
		long long truedest = newlayerstart + lastlayer*3 + 1;
		
		long long rotateddest = truedest + rotnum*rotated_potato[rownum];
		
		return account_for_potato(rownum,rotateddest);
	}
	
	inline long long account_for_potato(long long rownum, long long curidx){
		//Row N = X + feature_TC1idx * (1/2) * (3^N - 1) + feature_index*3^N + 1~3^N
		long long num = realtree[0];
		long long row_length = featuretc*ipow(3,rownum);
		long long minrowidx = num*featuretc + featuretc*(ipow(3,rownum)-1)/2; //inaccessible
		long long offset = (curidx - minrowidx-1) % row_length;
		if (offset < 0) offset +=row_length;
		return offset+minrowidx+1;
	}
	void update_ranks_add(long long featureindex, long long FR){
		for(long long n = 1; n <= realtree[0]; n++){
			long long target = 1 + (n-1)*featuretc + featureindex;
			if(realtree[target] >= FR) realtree[target]++;
		}
	}
	void update_ranks_subtract(long long featureindex, long long FR){
		for(long long n = 1; n <= realtree[0]; n++){
			long long target = 1 + (n-1)*featuretc + featureindex;
			if(realtree[target] > FR) realtree[target]--;
		}
	}
	long long node_group_min(long long rownum, long long groupbase,long long featureindex){
    	if(rownum == rows) return realtree[account_for_potato(rownum, groupbase)];
    	long long nextbase = get_childrenstartplace(rownum + 1, groupbase, featureindex);
    	return node_group_min(rownum + 1, nextbase, featureindex);
    }

	long long node_group_max(long long rownum, long long groupbase,long long featureindex){
    	if(rownum == rows) return realtree[account_for_potato(rownum, groupbase + 2)];
    	long long thirdkey = account_for_potato(rownum, groupbase + 2);
    	long long nextbase = get_childrenstartplace(rownum + 1, thirdkey, featureindex);
    	return node_group_max(rownum + 1, nextbase, featureindex);
    }	//recurse down

	void rotato_potato(){
		long long num = realtree[0];
		for(long long x = 1; x <= rows; ++x){
			long long row_length = featuretc*ipow(3,x);
			long long minrowidx = num*featuretc + featuretc*(ipow(3,x)-1)/2; //inaccessible
			long long maxrowidx = num*featuretc + featuretc*(ipow(3,x+1)-1)/2;
			vector<long long> vtemp;
			vtemp.assign(realtree.begin() + minrowidx+1, realtree.begin() + maxrowidx+1);
			
			long long actualrotation = rotated_potato[x] % row_length;
			
			rotate(vtemp.begin(),vtemp.end() - actualrotation,vtemp.end());
			
			copy(vtemp.begin(),vtemp.end(),realtree.begin() + minrowidx+1);
		}
		rotnum++;
	}
	void rotato_potato_v(long long x, vector<long long>& vin){
		//x is the rownum
		long long row_length = featuretc*ipow(3,x);
		long long actualrotation = rotnum*rotated_potato[x] % row_length;
		rotate(vin.begin(),vin.end()-actualrotation,vin.end());
		return;
	}
	
	void potato_rotato(){
    long long num = realtree[0];

    for(long long x = 1; x <= rows; ++x){
        long long minrowidx = num*featuretc +featuretc*(ipow(3,x)-1)/2;
        long long maxrowidx = num*featuretc +featuretc*(ipow(3,x+1)-1)/2;

        vector<long long> temp;
        temp.assign(realtree.begin() + minrowidx + 1,realtree.begin() + maxrowidx + 1);

        potato_rotato_v(x,temp); 
        copy(temp.begin(), temp.end(), realtree.begin() + minrowidx + 1);
    }

    rotnum = 0;
    }

	long long potato_rotato_s(long long x, long long idx){
		//x is the rownum
		long long num = realtree[0];
		long long row_length = featuretc*ipow(3,x);
		long long actualrotation = rotnum*rotated_potato[x] % row_length;
		long long reverserotation = row_length - actualrotation;
		
		return account_for_potato(x, idx - actualrotation);
	}
	
	void potato_rotato_v(long long x, vector<long long>& vin){
		//x is the rownum
		long long row_length = featuretc*ipow(3,x);
		long long actualrotation = rotnum*rotated_potato[x] % row_length;
		long long reverserotation = row_length - actualrotation;
		rotate(vin.begin(),vin.end()-reverserotation,vin.end());
		return;
	}
	

	void refresh_tree(long long featureindex){
		long long num = realtree[0];
		for(long long x = rows-1; x >= 1; --x){
			long long row_length = featuretc*ipow(3,x);
			long long minrowidx = num*featuretc + featuretc*(ipow(3,x)-1)/2; //inaccessible
			long long maxrowidx = num*featuretc + featuretc*(ipow(3,x+1)-1)/2;
			
			vector<long long> temp;
			temp.assign(realtree.begin() + minrowidx + 1, realtree.begin() + maxrowidx+1);
			potato_rotato_v(x,temp);
			
			for(long long y = 0; y < row_length/3; ++y){
				long long base = minrowidx + 1 + y*3;
				long long childstart = get_childrenstartplace(x+1,base,featureindex);
				auto it = temp.begin() + y*3;
				long long middlebase =account_for_potato(x + 1, childstart + 3);
				long long rightbase =  account_for_potato(x + 1, childstart + 6);

				*it       = node_group_max(x + 1, childstart, featureindex);
				*(it + 1) = node_group_min(x + 1, middlebase, featureindex);
				*(it + 2) = node_group_min(x + 1, rightbase, featureindex);
			}
			copy(temp.begin(),temp.end(),realtree.begin() + minrowidx + 1);
		}

	}
	

	
	
	void printsorted(long long featureindex){ //zero indexed features. 0 is time, 1 is value, 2+ is self defined
		long long num = realtree[0];
		vector<long long> temp;
		long long maxrowfeatidx = num*featuretc + featuretc*(ipow(3,rows)-1)/2 + featureindex*ipow(3,rows) + ipow(3,rows);
		long long minrowfeatidx = num*featuretc + featuretc*(ipow(3,rows)-1)/2 + featureindex*ipow(3,rows); //inaccessible
		long long minrowidx = num*featuretc + featuretc*(ipow(3,rows)-1)/2; //inaccessible
		long long maxrowidx = num*featuretc + featuretc*(ipow(3,rows+1)-1)/2;
		temp.assign(realtree.begin() + minrowidx + 1, realtree.begin() + maxrowidx + 1);
		
		potato_rotato_v(rows,temp);
		
		for(long long x = featureindex*ipow(3,rows);x< (featureindex+1)*ipow(3,rows) && temp[x] != inf;++x){
			cout<<temp[x]<<' ';
			
		}
		cout<<endl;
		rotato_potato();
	}
	
	void printranks(long long index){ //one-indexed
		
		for(long long x = (index)*featuretc+1; x<= (index+1)*featuretc; ++x){
			cout<<realtree[x]<<' ';
			
		}
		cout<<endl;
		rotato_potato();
	}
};

int32_t main(){
	vector<long long> b = {1,2,3,6,234,56,3,2,7,54,1,7,5};
	
	zdtree tree1(b);
	tree1.printsorted(1);
	tree1.printranks(5);
	
	vector<vector<long long>> a = {{1,2,3},{4,2,1},{6,5,8},{2,6,3}};
	zdtree tree2(a);
	tree2.printsorted(1);
	tree2.printranks(4);
	return 0;
}
