#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define M    541        /* Table size hesaplan�rken s�zl�kteki eleman say�s� 338, genel doluluk 
oran� da 0,6 olarak al�nd�. 343/0.65 = 527 i�leminden, tablo boyutu bu de�erden b�y�k en k���k asal say� olan
541 al�nd�.*/  
#define MM   540        // M-1 olarak al�nd�

int KelimeSayisiBul() //Dosyadaki kelimeleri tek tek oku ve toplam kelime sy�s�n� d�nd�r.
{
    int count;char ch[25];     
    FILE *fp=fopen("smallDictionary.txt","r");
    if(fp != NULL)
    {
        count=0;
        while(!feof(fp))
        {
            fscanf(fp,"%s",&ch);      
            count++;                    
        }
    }
    else
    printf("File is not found");
    fclose(fp);    
    return count; //Kelime say�s�n� d�nd�r
}
int findKey(char word[]) //Her kelime i�in bir Key de�eri hesapla
{
    int HashKey=0;
    int i;
    for(i=0;i<strlen(word);i++)
    {
        HashKey += (word[i]-'a')*26*i;
    }
    return HashKey;
}

int findIndex(int HashKey,int i) // DoubleHashing ile kelimeye ait bir hash de�eri hesaplan�yor
{
    int h1=HashKey%M;
    int h2=1+(HashKey%MM);
    int h=(h1+(i*h2))%M;
    return h;
}
int searchInHashTable(char hashTable[][25],char searchWord[]) //Kelimeyi hash tablosunda ara
{	
    int key=findKey(searchWord);                
    int i=0;
    int hashIndex;
    
    do /*Kelimeye ait indeksi bul ve tabloda o index dolu bak! Dolu ise aranan kelime mi; 
	de�ilse bir sonraki indise bak. Bo� g�rene kadar bakmaya devam et*/
    {
        hashIndex=findIndex(key,i);    
        if(!strcmp(hashTable[hashIndex],searchWord))   
        {
        	printf("------------------------------------------------------------------------\n");  
			printf("%d adimda bulundu\n",i+1);
            return hashIndex;            
        }
        else 
		{
		//	fflush(stdin);//?
		//	fflush(stdout);//?
		
		//	printf("%s\n",hashTable[hashIndex]);
			i++;                 
		}
	}while(hashTable[hashIndex][0]!='\0');    
    
	return -1;                        
}

void createHashTable(char hashTable[][25],char wordArray[][25],int wordCount) // Hash tablosunu olu�tur.
/*Tabloyu olu�tururken kelimenin key de�erini hesapla
Hesaplanan key de�erine g�re indis de�eri bul
Bulunan indis bo� ise kelimeyi yerle�tir dolu ise bir sonraki indise bak. Bo� g�rene kadar devam et. */
{
    int i,k;
    for(i=0;i<wordCount;i++)
    {

        int key=findKey(wordArray[i]);            
        int x=0;
        int indis=findIndex(key,x);        
        
        while(hashTable[indis][0]!='\0')            
        {
            x++;
            indis=findIndex(key,x);
        }
        
        for(k=0;k<strlen(wordArray[i]);k++)    
        {
            hashTable[indis][k]=wordArray[i][k];    
        }
        hashTable[indis][k]='\0';
    }
}
void kelimeOneri(char hashTable[][25],char searchWord[]) // Kelime �nerilerine bak!
{
	// Girilen kelimenin s�ra ile her harfini de�i�tir ve de�i�tirilen kelime hash tablosunda var m� bak!
	//Varsa �neri olarak ekrana yaz. Yoksa kelime �nerisi yoktur!
    int a,b,index;
    char temp;
    for(a=0;a<strlen(searchWord);a++)
    {
        temp=searchWord[a];                        
        for(b=0;b<26;b++)
        {
            searchWord[a]=b+97;                   
            index=searchInHashTable(hashTable,searchWord);   
			
           if(index>=0)
            {
                printf("WORD: %s\tHASH KEY: %d\t\tINDEX:%d\n",hashTable[index],findKey(searchWord),index);    
            }
        }
        searchWord[a]=temp;                       
    }
}

int main(void)
{
	int hashBoyutu=	KelimeSayisiBul(); //Dosyay� oku ve kelime say�s�n� bul
	printf("Kelime sayisi: %d\n",hashBoyutu);
	char wordArray[hashBoyutu][25]; // Bulunan kelime say�s�na g�re matris olu�tur. 
									//Sat�rlar kelimeleri s�tunlar o kelimenin uzunlu�unu bul!
	FILE *fp=fopen("smallDictionary.txt","r"); //Kelimeleri oku ve diziye at.
    if(fp != NULL)
    {
        int count=0;
        while(count<hashBoyutu)
        {
            fscanf(fp,"%s",wordArray[count]);   
            count++;
        }
    }
    else
    printf("File is not found");    
    fclose(fp);
    
    char hashTable[M][25];
    memset(hashTable,M,'\0');
    memset(hashTable[M],25,'\0');
    createHashTable(hashTable,wordArray,hashBoyutu);  
    
    char word[25];
    while(1)
    {    
    	fflush(stdin);
		fflush(stdout);
	    printf("\nKELIME GIR: ");
	    scanf("%s",word);	                      
	    int hashIndex=searchInHashTable(hashTable,word);        
	    if(hashIndex>=0)
	    {
	    	printf("------------------------------------------------------------------------\n");  
	        printf("\nBULUNAN KELIME: %s\t\tHASH KEY: %d\t\tINDEX: %d\n\n",word,findKey(word),hashIndex,hashTable[hashIndex]);
	        printf("------------------------------------------------------------------------\n");  
	    }
	    else
	    {
	    	fflush(stdin);
			fflush(stdout);
	    //	printf("------------------------------------------------------------------------\n");  
	        printf("KELIME YOK!\n");    
		//	printf("------------------------------------------------------------------------\n");  
			printf("\nONERILER: \n");
			printf("------------------------------------------------------------------------\n");  
	        kelimeOneri(hashTable,word);                  
	        printf("------------------------------------------------------------------------\n");  
	    }                                        
	}
    getch();
}
