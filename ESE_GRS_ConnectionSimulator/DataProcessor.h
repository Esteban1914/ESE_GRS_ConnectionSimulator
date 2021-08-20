#pragma once

class DataUnion
{
	union Union
{
public:
	double Double;
	float Float;
	int Int;
	unsigned Unsigned;
	short Short;
	char Char;
	char String[sizeof(double)];
};
public:
	Union u;
	unsigned Ultim,lengthDescodif;
	char*str;
public:
	DataUnion()
	{
		ResetStr();
		str=new char[1];
		lengthDescodif=0;
	}
	~DataUnion()
	{
		delete []str;
	}
	void ResetStr()
	{
		for(unsigned i=0;i<8;i++)
			u.String[i]=(char)0;
		Ultim=0;
	}
	void SetStr(const char*data,unsigned i,unsigned typeDato)
	{
		/*
		
		1: char
		2: short
	    4: int ; unsigned
		5: float
		8: double
		*/
		ResetStr();
		if(i<=8)
		{
			unsigned j=0;
			switch (typeDato)
			{
			case 1:
				if(i==1)
					u.String[0]=data[0];
				break;
			case 2:
			case 4:
				for(unsigned ii=0;ii<i;ii++)
					u.String[ii]=data[ii];
				break;
			case 5:
				for(unsigned ii=4-i;ii<4;ii++)
					u.String[ii]=data[j++];
				break;
			case 8:
				for(unsigned ii=8-i;ii<8;ii++)
					u.String[ii]=data[j++];
				break;
			default:
				break;
			}
			/*unsigned j=0;
			int k=7-(8-typeDato);
			for(unsigned ii=0;ii<i;ii++)
			{
				if(k<0||j>7)
					break;
				String[(unsigned)k--]=data[j++];
			}*/
		}
	}
	void SetDouble(double d)
	{
		ResetStr();
		u.Double=d;
		Ultim=6;
	}
	void SetFloat(float f)
	{
		ResetStr();
		u.Float=f;
		Ultim=5;
	}
	void SetInt(int i)
	{
		ResetStr();
		u.Int=i;
		Ultim=4;
	}
	void SetUnsigned(unsigned un)
	{
		ResetStr();
		u.Unsigned=un;
		Ultim=3;
	}
	void SetShort(short s)
	{
		ResetStr();
		u.Short=s;
		Ultim=2;
	}
	void SetChar(char ch)
	{
		ResetStr();
		u.Char=ch;
		Ultim=1;
	}
	////////CODIF////////////
	char* GetStrCodif()
	{
		delete[]str;
		str=new char[10];
		str[0]=(char)Ultim;
		unsigned j=1;
		for(unsigned i=0;i<8;i++)
		{
			if(u.String[i]!=(char)0)
			{
				str[0]+=(char)10;
				str[j++]=u.String[i];
			}
		}
		str[j]=0;
		return str;
	}
	void SetStrCodif(const char s[])
	{
		if(strlen(s))
		{
			ResetStr();
			char toDec[8];
			lengthDescodif=1;
			for(unsigned i=0;i<(unsigned)(s[0]/(char)10);i++)
			{
				toDec[i]=s[i+1];
				lengthDescodif++;
			}
			switch ((s[0]%(char)10))
			{
			case 1:
				SetStr(toDec,(s[0]/(char)10),sizeof(char));
				break;
			case 2:
				SetStr(toDec,(s[0]/(char)10),sizeof(short));
				break;
			case 3:
			case 4:
				SetStr(toDec,(s[0]/(char)10),sizeof(unsigned));
				break;
			case 5:
				SetStr(toDec,(s[0]/(char)10),sizeof(float)+1);
				break;
			case 6:
				SetStr(toDec,(s[0]/(char)10),sizeof(double));
				break;
			default:
				return;
				break;
			}
		}
	}
};
class DataProcessor
{
public:
	DataProcessor(){};
	~DataProcessor(){};
	static bool BitData(char Byte,unsigned posttion)
    {
	if((Byte & 0x01<<posttion)==0)
		return 0;
	return 1;

    };
	static bool PorcesarDatos(char ByteLow,char ByteHigh,float*angles)
{

	for(int i=2;i<8;i++)
	{
		switch (i)
		{
		  case 2:
		  case 3:
		  case 4:
		  case 5:
		  case 6:
		  case 7:
			    if(BitData(ByteLow,i)==1)
			       {
				     if(BitData(ByteHigh,i)==1)
						 angles[i-2]+=(float)0.9;
					 else
						 angles[i-2]-=(float)0.9;
					 RectificarAngules(angles);
			       }	
		         break;      	
	   }//end switch
	
	}//end for
	if(BitData(ByteHigh,1)==1)
	     return true;

	return false;

};
	static void RectificarAngules(float*angles){
	
	//comprobar angulos
	if(angles[0]>=360  )
		angles[0]-=360;
	else if(angles[0]<=-360)
		angles[0]+=360;
	if(angles[1]>=360  )
		angles[1]-=360;
	else if(angles[1]<=-360)
		angles[1]+=360;
	if(angles[2]>=360  )
		angles[2]-=360;
	else if(angles[2]<=-360)
		angles[2]+=360;
	if(angles[3]>=360  )
		angles[3]-=360;
	else if(angles[3]<=-360)
		angles[3]+=360;
	if(angles[4]>=360  )
		angles[4]-=360;
	else if(angles[4]<=-360)
		angles[4]+=360;
	if(angles[5]>=360  )
		angles[5]-=360;
	else if(angles[5]<=-360)
		angles[5]+=360;


};
	static char* printfBits(char c)
{
	char*f=new char[9];
	f[8]=0;
	for(int i=0;i<8;i++)
	  {
		  if(BitData(c,i)==0)
			  f[7-i]='0';
		  else
			  f[7-i]='1';
	  }
		return f;
};
	static bool CodigoCliente(char lowByte,char hightByte)
	{
		if(CodigoESE(lowByte,hightByte)&&DataProcessor::BitData(lowByte,1)==1&&lowByte<(char)35)
			return true;
		return false;	
	}
	static bool CodigoServer(char lowByte,char hightByte)
	{
		
		if(CodigoESE(lowByte,hightByte)&&DataProcessor::BitData(lowByte,1)==1&&lowByte>=(char)35)
			return true;
		if(lowByte==(char)59||lowByte==(char)115)
			return true;
		return false;	
	}
	static bool CodigoSeguridad(char lowByte,char hightByte)
	{
		if(lowByte==(char)4&&hightByte==(char)1)
			return true;
		return false;
	}
	
	static bool CodigoESE(char lowByte,char hightByte)
	{
		if(DataProcessor::BitData(lowByte,0)==1&&DataProcessor::BitData(hightByte,0)==1)
			return true;
		return false;
	}	
	/////WebSocket
	static bool GetBit(char Byte,unsigned posittion)
    {
	if((Byte & 0x01<<posittion)==0)
		return 0;
	return 1;

    };
	static char* printByte(char c)
	{
		char*f=new char[9];
		f[8]=0;
		for(int i=0;i<8;i++)
		{
			if(GetBit(c,i)==0)
				f[7-i]='0';
			else
				f[7-i]='1';
		}
		return f;
	};
	static void SetBit(char&Byte,unsigned posittion,bool bit)
	{
		if(bit)
			Byte=(Byte | (0x01<<posittion));
		else
		{
			char _256=(char)255;
			_256=(_256^(0x01<<posittion));
			Byte=(Byte & _256);
		}
	}
	static char SetByte(char Byte[8])
	{
	char By=0x00;
	if(strlen(Byte)==8)
	{
		for(int i=7;i>=0;i--)
		{
			if(Byte[7-i]=='1')
				SetBit(By,i,true);
		}
	}
	return By;
	}
	static unsigned GetDataByStr(char*Str,unsigned CantBytes)
	{

	unsigned multip=1,ToReturn=0;
	int i=7*CantBytes;
	for(i;i>=0;i--)
	{
		if(Str[i]=='1')
			ToReturn+=multip;
		multip*=2;
	}
	return ToReturn;
	}
	static unsigned long long GetDataByBinary(char*Binary,unsigned CantBytes)
	{

	unsigned long long multip=1,ToReturn=0;
	int i=7;
	for(int ii=(int)(CantBytes-1);ii>=0;ii--)
		for(i;i>=0;i--)
		{
			if(GetBit(Binary[ii],7*CantBytes-i))
				ToReturn+=(unsigned long long)multip;
			multip*=2;
		}
	return ToReturn;
	}
};

