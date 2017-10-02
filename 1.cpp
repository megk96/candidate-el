#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <iterator>
#include <set>
#include <map>
#include <iostream>
#include <sstream>
#include <deque>
#include <cmath>
#include <memory.h>
#include <algorithm>
#include <utility>
#include <climits>

using namespace std;

class CSVRow
{
    public:
        string const& operator[](size_t index) const
        {
            return m_data[index];
        }
        size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(istream& str)
        {
            string         line;
            getline(str, line);

            stringstream   lineStream(line);
            string         cell;

            m_data.clear();
            while(getline(lineStream, cell, ','))
            {
                m_data.push_back(cell);
            }
            if (!lineStream && cell.empty())
            {
                m_data.push_back("");
            }
        }
    private:
        vector<string>    m_data;
};

istream& operator>>(istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}   
int main()
{
    ifstream       file("zoo.data");

    CSVRow              row;
    vector <string> curr(18), temp(17);
	    set <vector<string> > specific, generic;
	    for(int j=1;j<17;j++)
	      {
		temp[j].assign("null");
	      }
	    specific.insert(temp);
	    for(int j=1;j<17;j++)
	      {
		temp[j].assign("all");
	      }
	    generic.insert(temp);
     for(int i=1;i<8;i++)
      {
	string s;
	stringstream ss;
	ss<<i;
	s = ss.str();
	while(file >> row)
	  {
	   
	    for(int j=0;j<18;j++)
	      {
		curr[j].assign(row[j]);
	      }
	    
	       vector<string> m, n, p;
	       if(!curr[17].compare(s))
    //if positive example
    {
      //remove inconsistent hypotheses from generic border
      set < vector<string> >tg, ts;
      tg = generic;
      for(set<vector<string> >::iterator it= generic.begin(); it!=generic.end(); it++)
      {
        m = *it;
        int flag =0;
        for(int j=1; j<17; j++)
        {
	  if((m[j].compare(curr[j])!=0)&&(m[j].compare("all")!=0))
                  {
            flag = 1;
            break;
          }
        }
        if(flag==1)
          tg.erase(m);
      }
      generic = tg;

      m = *(specific.begin());
      n = *(specific.begin());
      specific.erase(n);

      for(int j=1; j<17; j++)
      {
	if(m[j].compare("null")==0)
               {
		 m[j].assign(curr[j]);
        }
	else if(m[j].compare(curr[j])!=0)
        {
          m[j].assign("all");
        }
      }

      specific.insert(m);
    }
	        else //if negative example
    {
      /*
      if example is inconsistent with spec border, then it is noise
      */

      set < vector<string> > tg, ts;
      n = *(specific.begin());

      for(set< vector<string> >::iterator it= generic.begin(); it!=generic.end(); it++)
      {
        m = *it;

        int flag=0;
        //check if given example is consistent with the present hypothesis
        for(int j=1; j<17; j++)
        {
	   if((m[j].compare(curr[j])!=0)&&(m[j].compare("all")!=0))
          {
            flag=1; //curr hyp is consistent
            break;
          }
	   
        }
	  if(flag==1)//if hyp is consistent with the example
        {
          tg.insert(m);
        }
        else//hyp is not consistent with the example
        {
          vector<string> temphyp;
          for(int j=1; j<17; j++)
          {
            if(!m[j].compare("all"));
            {
              temphyp = m;
	      if(j==13)
		{
		  for(int k=0;k<=8;k=k+2)
		    {
		      string ss;
		      stringstream sss;
		      sss<<k;
		      ss = sss.str();
		      if(!ss.compare(curr[j]))
			continue;
		      else
			{
			  temphyp[j].assign(ss);
                  
                  //check if temphyp is more general than specifc hyp.
                bool consistent = true;
                  for(int l=1; l<17; l++)
                  {
		    if((temphyp[l].compare("all")!=0)&&(temphyp[l].compare(n[l])!=0)&&(n[l].compare("null")!=0))
                  {
                      consistent = false;
                      break;
                    }
                  }
                  if(consistent)
                    tg.insert(temphyp); // new hypo is consistent
			}
		    }
		}
	      else
			{
		  for(int k=0;k<2;k++)
		    {
		      string ss;
		      stringstream sss;
		      sss<<k;
		      ss = sss.str();
		      if(!ss.compare(curr[j]))
			continue;
		      else
			{
			  temphyp[j].assign(ss);
                  
                  //check if temphyp is more general than specifc hyp.
                bool consistent = true;
                  for(int l=1; l<17; l++)
                  {
		    if((temphyp[l].compare("all")!=0)&&(temphyp[l].compare(n[l])!=0)&&(n[l].compare("null")!=0))
                  {
                      consistent = false;
                      break;
                    }
                  }
                  if(consistent)
                    tg.insert(temphyp); // new hypo is consistent
			}
		    }
		}
          
	    }
        }

      }
      }

      generic.clear();
      bool mGen;
      set<vector<string> > tempgg;
      
      //remove from generic any hyp that is more specific than another hyp in generic
      for(set< vector<string> >::iterator it= tg.begin(); it!=tg.end(); it++)
      {
        m = *it;

        for( set< vector<string> >::iterator jt= tg.begin(); jt!=tg.end(); jt++ )
        {
          if(it==jt)
            continue;

          p = *jt;
         bool  consistent = true;
          for(int k=1; k<17; k++)
          {
	    if((m[k].compare("all")!=0)&&(m[k].compare(p[k])!=0))
            {
              consistent = false;
              break;
            }
          }
          if(consistent)
            tempgg.insert(p);
        }
      }

      // generic = set_difference(tempg, tempgg );
      for( set< vector<string> >::iterator it= tg.begin(); it!=tg.end(); it++ )
      {
        m = *it;

        if(tempgg.find(m)==tempgg.end())
        {
          generic.insert(m);
        }
      }
    }
	  }
  vector<string> abc;

 cout<<"specific border"<<endl;
  for(set< vector<string> >::iterator it= specific.begin(); it!=specific.end(); it++)
  {
    abc = *it;
 
  cout<<"< ";
  for(int j=1; j<17; j++)
  {
    if(abc[i].compare("all")==0)
        cout<<"?"<<" ";
    else if(abc[i].compare("null")==0)
	       cout <<"@"<<" ";
    else
    {
      cout << abc[i] << " ";
    }
	       }
  }
  cout<<">\n";

  cout<<"\ngeneric border"<<endl;
  
  for(set< vector<string> >::iterator it= generic.begin(); it!=generic.end(); it++)
  {
    abc = *it;
 
  cout<<"< ";
  for(int j=1; j<17; j++)
  {
    if(abc[i].compare("all")==0)
        cout<<"?"<<" ";
    else if(abc[i].compare("null")==0)
	       cout <<"@"<<" ";
    else
    {
      cout << abc[i] << " ";
    }
	       }
  }
    cout<<">\n";
  cout<<"\ncandidate elem done! :)\n";
	

      }
      
    
    return 0;
}
