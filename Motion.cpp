 #include <iostream>
#include <cstdlib>
#include <string>

using namespace std;


bool isItADigit  (char a)
{
    if (a >='0' && a <='9')
        return true;
    else
        return false;
}

bool isItALetter (char a)
{
    if (a=='a' || a=='A' || a=='s' || a=='S' || a=='d' || a=='D' || a=='w' || a=='W')
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool isMotionMeaningful(string motion)
{
    int i;
    //cerr << motion.length()<<endl;
    if (motion.length() == 0)
        return true;
    else
    {
        for (i=0; i< motion.length(); i++)
        {
            if (!(isItADigit (motion[i]) || isItALetter(motion[i]) || motion[i] == '/')) //if is not a direction or a digit or a slash
            {
                cerr << "undefined input\n";
                return false;
            }
            
            else if (isItALetter(motion[i]))  //if i is a direction
            {
                if (motion[i+1] !='/')
                {
                    cerr << "direction not followed by a slash\n";
                    return false;
                }
            }
            
            else if(isItADigit(motion[i]))  //if i is a digit
            {
                if (isItADigit(motion[i+1]))  //if i+1 is a digit
                {
                    if (isItALetter(motion[i+2]))  // if i+2 is a direction
                    {
                        if (motion[i+3]!='/')   //if not followed by a slash
                        {
                            cerr << "2 successive digit + one direction not followed by a slash\n";
                            return false;
                        }
                    }
                    else   // if i+2 is not a direction
                    {
                        cerr << "2 successive digit not followed by a direction\n";
                        return false;
                    }
                }
                    
                else if (motion[i+1] == '/')  //if i+1 is a slash
                {
                    cerr << "digit followed by neither a letter nor another digit\n";
                    return false;
                }
                
                else // if i+1 is a direction
                {
                    if (motion[i+2] !='/')  // if not followed by a slash
                    {
                        cerr << "a number and a letter not followed by a slash\n";
                        return false;
                    }
                }
            }
            
        }
        return true;
    }
    
}


bool isSlashNumberNotCorrect (string motion, int lengthOfBeat, int skip, int startPoint, int &wrongBeat, int count)  //check error 2
{
    for (int j=0; j<lengthOfBeat; j++)
    {
        if(motion[startPoint+skip+j] != '/')
        {
            if (isItADigit(motion[startPoint+skip+j]) || isItALetter(motion[startPoint+skip+j]))
            {
                wrongBeat = count+j+1;
                return true;
            }
        }
    }
    return false;
}


bool endPremature(string motion, int &endPrematureBeat, int count)
{
    for (int k= int (motion.size()-1); k>=0; k--)
    {
        if (motion[k] != '/')   //isn't a slash, i.e.a direction
        {
            if (isItADigit(motion[k-2]))    //2 digits
            {
                int length = int (motion.size()-1-k);
                if ((10*(motion[k-2]-'0')+(motion[k-1]-'0')) > length)  //if hold is larger than slash numbers
                {
                    endPrematureBeat = count+1;
                    return true;
                }
                else
                    return false;
            }
            else   //one digit
            {
                int length = int (motion.size()-1-k);
                if ((motion[k-1]-'0') > length)
                {
                    endPrematureBeat = count+1;
                    return true;
                }
                else
                    return false;
            }
        }
    }
    return false;
}


int translateMotion(string motion, string& instructions, int& badBeat)
{
    if (isMotionMeaningful(motion))
    {
        int count=0;
        cerr << "motion meaningful\n";
        for (int i=0; i<motion.length(); i++)  //check if translatable
        {
            if (isItADigit(motion[i]))
            {
                if (motion[i]=='0')
                {
                    //if (!(motion[i+1] >='2' && motion[i+1] <= '9'))
                    if (isItALetter(motion[i+1]) || motion[i+1]=='0' || motion[i+1]=='1')  // eg. "0a/" or "00a/" 0r "01a/"
                    {
                        cerr << "error: length <= 1 " << endl;
                        badBeat=count+1;
                        cerr << "bad beat: " << badBeat << endl;
                        return 4;
                    }
                    
                    else   //  0 followed by a digit larger than 1 (length>2);
                    {
                        int length = motion[i+1]-'0';
                        int skip = 3;
                        if (isSlashNumberNotCorrect(motion, length, skip, i, badBeat, count))  //check error 2
                        {
                            cerr << "first 0, error 2" << endl;
                            return 2;
                        }
                        
                        else
                        {
                            i++;   //if nothing wrong, skip the second digit examination
                        }
                    }
                }
                
                else if (motion[i]=='1')
                {
                    if (!isItADigit(motion[i+1]))   //if 1 followed by a direction
                    {
                        cerr << "length=1 with 1 infront of the direction\n";
                        badBeat = count+1;
                        return 4;
                    }
                    else  //if 1 followed by a digit
                    {
                        int length = 10 + motion[i+1]-'0';
                        int skip = 3;
                        if (isSlashNumberNotCorrect(motion, length, skip, i,badBeat, count))  //check error 2
                        {
                            cerr << "1X, error 2 " << endl;
                            return 2;
                            
                        }
                    
                        else
                        {
                            i++;   //skip the second digit examination
                        }
                    }
                }
                
                else if (isItALetter(motion[i+1]))  //a digit(>1) followed by a direction
                {
                    int length = motion[i]-'0';
                    int skip = 2;
                    if (isSlashNumberNotCorrect(motion, length, skip, i, badBeat, count))
                    {
                        cerr << "4a/, error 2" << endl;
                        return 2;
                    }
                }
                
                else if(isItADigit(motion[i+1]))  //2 digits followed by a direction
                {
                    int length = 10*(motion[i]-'0')+(motion[i+1]-'0');
                    int skip = 3;
                    if (isSlashNumberNotCorrect(motion, length, skip, i, badBeat, count))
                    {
                        cerr << "23a/, error 2" << endl;
                        return 2;
                    }
                    
                    else
                    {
                        i++;   //if nothing wrong, skip the second digit examination
                    }
                }
            }
            
            
            else if (motion[i]=='/') //if is a slash
            {
                count ++;
            }
            
        }
        
        if (endPremature(motion, badBeat, count))
        {
            return 3;
        }
        
        else   //have checked all conditions, if translatable, translate motion
        {
            int i=0;
            while (i<motion.length())
            {
                if (motion[i]=='/')     //translate slash into '.'
                {
                    instructions +='.';
                    i++;
                }
                
                else if (isItALetter(motion[i]))   //translate uppercase direction into lower case
                {
                    instructions += tolower(motion[i]);
                    i += 2;   //skip the next slash translation
                }
                
                else                               // encounter a digit
                {
                    if (isItADigit(motion[i+1]))   //if 2 successive digits
                    {
                        int length = 10*(motion[i]-'0')+(motion[i+1]-'0');
                        for (int j=0; j<length; j++)
                        {
                            instructions += toupper (motion[i+2]);
                        }
                        i += length+3;  // skip the translation of the following (length+2) characters
                    }
                    else         //if 1 digit
                    {
                        int length = motion[i]-'0';
                        for (int j=0;j<length; j++)
                        {
                            instructions += toupper(motion[i+1]);
                        }
                        i += length+2;   //skip the translation of the following (length+1) characters
                    }
                }
            }
            
            return 0;
        }
        
    }
  
    else   //not meaningful
    {
        cerr << "motion not meaningful\n";
        return 1;
    }
}


int main()
{
    string result;
    string motion;
    int badBeat;
    getline (cin, motion);
    translateMotion(motion, result, badBeat);
    cerr << "intstructions: " << result << endl;
    cerr << "bad beat: " << badBeat << endl;
    cerr << "is Motion Meaningful: " << isMotionMeaningful(motion) << endl;
    cerr << "return value: " << translateMotion(motion, result, badBeat) << endl;
    return 0;
}

