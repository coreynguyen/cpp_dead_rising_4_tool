//#define _IL_BUILD_LIBRARY

#define wdirent _wdirent
#define wDIR _WDIR
#define wopendir _wopendir
#define wreaddir _wreaddir
#define wclosedir _wclosedir
#define wrewinddir _wrewinddir


#include <iostream>
#include <string>
#include <sstream>
#include <math.h>       /* pow */
#include <windows.h>		// add linker option: -lwinmm    -lcomdlg3    -lole32
#include <shlwapi.h>		// add linker option: -lshlwapi
#include <stdint.h>
#include <direct.h>
#include <dirent.h>
#include <zlc/zlibcomplete.hpp>	// add linker option: -lzlibcomplete    -lz
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h> //Used for loading the textures

#include "resource.h"
#include "img_dds.h"

/*
char array_biting_chip[4] = {0, 0, 0, 0};
char array_damn_batteries_dead[4] = {0, 0, 0, 0};
char array_dont_go[4] = {0, 0, 0, 0};
char array_ive_covered_wars_u_know[4] = {0, 0, 0, 0};
char array_just_dont_stand_there[4] = {0, 0, 0, 0};
char array_right_on[4] = {0, 0, 0, 0};
char array_theres_a_party[4] = {0, 0, 0, 0};
char array_time_to_go[4] = {0, 0, 0, 0};
char array_good_bye[4] = {0, 0, 0, 0};
char array_yaa_now_were_talkin[4] = {0, 0, 0, 0};
*/

#include "audio_biting_chip.h"
#include "audio_damn_batteries_dead.h"
#include "audio_dont_go.h"
#include "audio_ive_covered_wars_u_know.h"
#include "audio_just_dont_stand_there.h"
#include "audio_right_on.h"
#include "audio_theres_a_party.h"
#include "audio_time_to_go.h"
#include "audio_good_bye.h"
#include "audio_yaa_now_were_talkin.h"


using namespace zlibcomplete;
using namespace std;

template <typename T> string NumberToString(T pNumber) {
	ostringstream oOStrStream;
	oOStrStream << pNumber;
	return oOStrStream.str();
	}

struct fmtBctInfo {
	unsigned long dataAddr;		// should start at a 16 byte aligned position
	unsigned long dataSize;	// this will change based on the image format
	unsigned long unk7;	// Flag likely, seems to always be 0x80000000
	unsigned long unk8;	// always0?
	};
struct fmtBctHeader {
	unsigned char unk1;	// ?, Always 0x07
	unsigned char unk2;	// ?, Always 0x01
	unsigned char unk3;	// ?, Always 0x02
	unsigned char unk4;	// ?, Range {0x20 0x60 0xE0 0xE8} ?? No clue what this is
	unsigned short imgWidth;
	unsigned short imgHeight;
	unsigned char imgFormat;
	unsigned char unk5;		// version? always 0xFF in DR4, DR2 uses 0x63
	unsigned char imgMips;	// ? Mip Map Count
	unsigned char imgBitsPerPixel;
	unsigned long imgHash;	// seems to be calculated on the image data. dont know how to accurately recalculate this
	unsigned long imgInfoAddr;
	/*
	Unknown Data Block
	Data maybe dynamic based on values
	supplied earlier in the header

	will have to store the data to
	an char array for now..


	edit:
	after looking more into it, the block appears to
	always stay the same lol
	16bytes
	20000000000000000000000020000100
	*/
	char* unk9_buffer;
	unsigned long unk9_count;
	fmtBctInfo* imgInfo;
	// Pad the end of header to 16bytes
	fmtBctHeader () {
		unk9_buffer = NULL;
		unk9_count = 0;
		imgInfo = NULL;
		imgMips = 0;
		}
	~fmtBctHeader () {
		if (unk9_buffer != NULL) {
			delete[] unk9_buffer;
			unk9_buffer = NULL;
			}
		unk9_count = 0;
		if (imgInfo != NULL) {
			delete[] imgInfo;
			imgInfo = NULL;
			}
		imgMips = 0;
		}
	};

struct fmtBigTable {
	std::string name;
	unsigned long string_addr;
	unsigned long hash_id;
	unsigned long com_size;
	unsigned long raw_size;
	unsigned long addr;
	unsigned long unk;
	unsigned long flag;
	/*
	I'm unable to recalculate the hash

	so I'm gonna try and re-calculate it
	using various algo's and test it
	against the source hash id later

	Just gonna store my new hashes
	in the below variables..
	*/
	unsigned long hash_sdbm;
	unsigned long hash_djb2;
	};

struct fmtBigHeader {
	unsigned long id;
	unsigned long data_addr;
	unsigned long size;
	unsigned long count;
	unsigned long table_addr;
	unsigned long string_table_addr;
	fmtBigTable* table;
	fmtBigHeader () {
		id = 0;
		data_addr = 0;
		size = 0;
		count = 0;
		table_addr = 0;
		string_table_addr = 0;
		table = NULL;
		}
	~fmtBigHeader () {
		if(table != NULL) {delete[] table;}
		}
	};

struct fmtTexTable1 {
	unsigned long NameOff;
	unsigned long unk05;
	unsigned long zsize;
	unsigned long size;
	unsigned long offset;
	unsigned long type;
	unsigned long zip;
	};
struct fmtTexTable2 {
	unsigned long ukn01;
	unsigned long ukn02;
	unsigned long ukn03;
	unsigned long ukn04;
	unsigned long texH;
	unsigned long texW;
	unsigned long texC;
	unsigned long ukn05;
	unsigned long texM;
	unsigned long ukn06;
	unsigned long ukn07;
	unsigned long mtable;
	unsigned long tOffset;
	unsigned long tSize;
	};
struct fmtTexHeader {
	unsigned long fileID;	// 0x03040506
	unsigned long data_addr;
	unsigned long file_size;
	unsigned long file_count;
	unsigned long table_addr;
	unsigned long string_addr;
	std::string* string_table;
	fmtTexHeader () {string_table = NULL;}
	};

std::wstring utf8_2_unicode (std::string str) {
	/*
	Uh this is a hack, cause I dont know how to read
	unicode from windows.

	I was told I need to enable the unicode flag in
	Microsoft Visual Studios, uh but I dont use that
	sooo

	I'm just guessing that strings are all UTF8
	and I can just convert them to unicode???
	*/
	std::wstring w;
	uint8_t byte;
	uint32_t uni;

	unsigned long i = 0;
	while (i < str.size()) {
		byte = str[i];
		if (byte <= 0x7F) {
			w += str[i];
			}
		else if (byte <= 0xDF) {
			uni = (((str[i] & 0x1F) << 6) & ((str[i + 1] & 0x3F)));
			w += uni;
			i++;
			}
		else if (byte <= 0xEF) {
			uni = (((str[i] & 0x0F) << 12) | ((str[i + 1] & 0x3F) << 6)  |  ((str[i + 2] & 0x3F) )  );
			w += uni;
			i+=2;
			}
		else if (byte <= 0xF7) {
			uni = (((str[i] & 0x07) << 18) | ((str[i + 1] & 0x3F) << 12)  |  ((str[i + 2] & 0x3F) << 6)    |  ((str[i + 3] & 0x3F) )  );
			w += uni;
			i+=3;
			}
		i++;
		}
	return w;
	}

bool openFile (char* &f, size_t &fsize, std::wstring filename) {
	/*
	opens a file in wide mode, stores bytes to a char array
	*/
	//wcout << filename << endl;
	HANDLE hFile = CreateFileW(
		filename.c_str(),		// file to open
		GENERIC_READ,			// open for reading
		FILE_SHARE_READ,		// share for reading
		NULL,					// default security
		OPEN_EXISTING,			// existing file only
		FILE_ATTRIBUTE_NORMAL,	// normal file
		NULL					// no attr. template
		);
	// check that file was opened successfully
	if (hFile == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to Open File\n";
		return false;
		}
	// get / check file size is larger then 0
	fsize = GetFileSize(hFile, NULL);
	if (fsize == 0) {
		std::cout << "Failed to read file. File is Empty?\n";
		return false;
		}
	// size char for buffer
	f = new char[fsize];
	// attempt to read file contents into memory
	unsigned long dwBytesRead = 0;
	if(ReadFile(hFile, f, fsize, &dwBytesRead, NULL) == FALSE || dwBytesRead != fsize) {
		std::cout << "Failed to copy file into memory\n";
		fsize = 0;
		delete[] f;
		f = NULL;
		CloseHandle(hFile);
		return false;
		}
	// close out handle :-)
	CloseHandle(hFile);
	}

bool writeFile (char* f, size_t fsize, std::wstring filename, size_t write_pos = 0, size_t write_size = 0) {
	//DeleteFileW(filename.c_str());
	HANDLE hFile;
	unsigned long dwBytesWritten = 0;
	unsigned long dwPos = 0;
	hFile = CreateFileW(
		filename.c_str(),	// open One.txt
		GENERIC_WRITE,						// open for reading
		0,									// do not share
		NULL,								// no security
		CREATE_ALWAYS,						// creates new file
		FILE_ATTRIBUTE_NORMAL,				// normal file
		NULL								// no attr. template
		);
	if (hFile == INVALID_HANDLE_VALUE) {
		std::cout << "Error:\t" << hFile << std::endl;
		return false;
		}
	dwPos = SetFilePointer(hFile, 0, NULL, FILE_END);
	if (write_size == 0) {write_size = fsize - write_pos;}
	LockFile(hFile, dwPos, 0, write_size, 0);
	WriteFile(hFile, f + write_pos, write_size, &dwBytesWritten, NULL);
	UnlockFile(hFile, dwPos, 0, write_size, 0);
	CloseHandle(hFile);
	return true;
	}

void storelong (std::string &f, size_t pos, unsigned long val, bool swap_order = false) {
	if (swap_order) {
		f[pos + 3] = val & 0x00FF;
		f[pos + 2] = (val & 0xFF00) >> 0x08;
		f[pos + 1] = (val & 0xFF0000) >> 0x10;
		f[pos + 0] = (val & 0xFF000000) >> 0x18;
		}
	else {
		f[pos + 3] = (val & 0xFF000000) >> 0x18;
		f[pos + 2] = (val & 0xFF0000) >> 0x10;
		f[pos + 1] = (val & 0xFF00) >> 0x08;
		f[pos + 0] = val & 0x00FF;
		}
	}

unsigned char readbyte (char* &f, size_t &pos) {
	unsigned char result = (uint8_t)f[pos];
	pos += 1;
	return result;
	}

unsigned short readshort (char* &f, size_t &pos) {
	unsigned short result = {
		(unsigned int)((uint8_t)f[pos + 0] * 0x00000001) + \
		(unsigned int)((uint8_t)f[pos + 1] * 0x00000100)
		};
	pos += 2;
	return result;
	}

unsigned long readlong (char* &f, size_t &pos) {
	unsigned long result = {
		(unsigned int)((uint8_t)f[pos + 0] * 0x00000001) + \
		(unsigned int)((uint8_t)f[pos + 1] * 0x00000100) + \
		(unsigned int)((uint8_t)f[pos + 2] * 0x00010000) + \
		(unsigned int)((uint8_t)f[pos + 3] * 0x01000000)
		};
	pos += 4;
	return result;
	}

void writelong (char* &f, size_t pos = 0, unsigned long val = 0, bool swap_order = false) {
	if (swap_order) {
		f[pos + 3] = val & 0x00FF;
		f[pos + 2] = (val & 0xFF00) >> 0x08;
		f[pos + 1] = (val & 0xFF0000) >> 0x10;
		f[pos + 0] = (val & 0xFF000000) >> 0x18;
		}
	else {
		f[pos + 3] = (val & 0xFF000000) >> 0x18;
		f[pos + 2] = (val & 0xFF0000) >> 0x10;
		f[pos + 1] = (val & 0xFF00) >> 0x08;
		f[pos + 0] = val & 0x00FF;
		}
	}

void writeshort (char* &f, size_t pos = 0, unsigned long val = 0, bool swap_order = false) {
	if (swap_order) {
		f[pos + 1] = val & 0x00FF;
		f[pos + 0] = (val & 0xFF00) >> 0x08;
		}
	else {
		f[pos + 1] = (val & 0xFF00) >> 0x08;
		f[pos + 0] = val & 0x00FF;
		}
	}


ULONGLONG get_file_size(const char* filename) {
	WIN32_FILE_ATTRIBUTE_DATA file_info;
	if (!GetFileAttributesExA(filename, GetFileExInfoStandard, &file_info)) {
		// handle error here
		return 0;
	}
	return file_info.nFileSizeLow | ((ULONGLONG)file_info.nFileSizeHigh << 32);
}


size_t fileSizeFromAttr(std::wstring name) {
    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (!GetFileAttributesExA((char*)name.c_str(), GetFileExInfoStandard, &fad))
        return -1; // error condition, could call GetLastError to find out more
    LARGE_INTEGER size;
    size.HighPart = fad.nFileSizeHigh;
    size.LowPart = fad.nFileSizeLow;
    return size.QuadPart;
	}


std::string ucase(const std::string &s) {
	std::string ret(s.size(), char());
	for (unsigned int i = 0; i < s.size(); ++i) {
		ret[i] = (s[i] <= 'z' && s[i] >= 'a') ? s[i]-('a'-'A') : s[i];
		}
	return ret;
	}

std::string lcase(const std::string &s) {
	std::string ret(s.size(), char());
	for (unsigned int i = 0; i < s.size(); ++i) {
		ret[i] = (s[i] <= 'Z' && s[i] >= 'A') ? s[i]-('A'-'a') : s[i];
		}
	return ret;
	}

std::wstring wlcase(const std::wstring &s) {
	std::wstring ret(s.size(), char());
	for (unsigned int i = 0; i < s.size(); ++i) {
		ret[i] = (s[i] <= 'Z' && s[i] >= 'A') ? s[i]-('A'-'a') : s[i];
		}
	return ret;
	}

std::string readstring (char* &f, size_t &pos, size_t fsize, int length = -1) {
	std::string result = "";
	unsigned char letter;
	if (length < 0) {
		while (pos < fsize) {
			letter = f[pos];
			pos++;
			if (letter == 0) {break;}
			result += letter;
			}
		}
	else {
		for (int i = 0; i < length; ++i) {
			letter = f[pos];
			pos++;
			if (pos > fsize || letter == 0) {break;}
			result += letter;
			}
		}
	return result;
	}

std::wstring openfilenameW(const wchar_t* filter = L"All Files (*.*)\0*.*\0", HWND owner = NULL, unsigned long flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY) {
	// add to link -lcomdlg32 -lole32
	std::wstring filename(MAX_PATH, L'\0');
	OPENFILENAMEW ofn = { };
	//ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn); //sizeof(LPOPENFILENAMEW);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = &filename[0];  // use the std::wstring buffer directly
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = L"Select a File";
	ofn.Flags = flags;
	//OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_EXPLORER| OFN_ALLOWMULTISELECT|OFN_NOCHANGEDIR|OFN_HIDEREADONLY| OFN_SHAREAWARE;
	if (!GetOpenFileNameW(&ofn)) {return L"";}
    //std::size_t firstScan = filename.find_first_not_of("\0");
    //std::size_t first = 0;
    //if (firstScan == std::string::npos) {first = filename.length();}
	//else {first = firstScan;}
    //std::size_t last = filename.find_last_not_of("\0");
	//return filename.substr(first, last-first+1);
	return filename;
	}

std::string getFilenamePath (const std::string str) {
	// getFilenamePath file   -- returns: "g:\subdir1\subdir2\"
	size_t found;
	std::string strt;
	found = str.find_last_of("/\\");
	if (found != std::string::npos) {
		strt = str.substr(0, found);
		return (strt + "\\");
		}
	else {
		return "";
		}
	}

std::string getFilenameFile(const std::string str) {
	// getFilenameFile file   -- returns: "myImage"
	size_t found;
	std::string strt;
	found = str.find_last_of("/\\");
	if (found < str.size()) {
		strt = str.substr(found + 1, -1);
		found = strt.find(".");
		if (found < strt.size()) {
			strt = strt.substr(0, found);
			}
		//return strt;
		}
	else {
		strt = str;
		//return str;
		}
	size_t lastdot = strt.find_last_of(".");
	if (lastdot == std::string::npos) return strt;
	return strt.substr(0, lastdot);
	}

std::string getFilenameType (std::string const &path ) {
	// getFilenameType file -- returns: ".jpg"
	std::string ext;
	size_t dotIdx = path.find_last_of( "." );
	if ( dotIdx != std::string::npos ) {
		size_t dirSepIdx = path.find_last_of( "/\\" );
		if ( dotIdx > dirSepIdx + 1 ) {
			ext = path.substr( dotIdx );
			}
		}
	return ext;
	}

std::string bitIntAsString (double a) {
	std::string result = "";
	std::ostringstream temp;
	temp << a;
	if (temp.good ()) {
		result = temp.str();
		}
	return result;
	}

std::string bitIntAsHex(int number, int length = 2) {
	std::string s;
	std::ostringstream temp;
	temp << std::hex << number;
	s = ucase(temp.str());
	s.insert(s.begin(), length - s.length(), '0');
	return ("0x" + s);
	}

std::wstring wgetFilenamePath (std::wstring str) {
	// getFilenamePath file   -- returns: "g:\subdir1\subdir2\"
	size_t found;
	std::wstring strt;
	found = str.find_last_of(L"/\\");
	if (found != std::wstring::npos) {
		strt = str.substr(0, found);
		return (strt + L"\\");
		}
	else {
		return L"";
		}
	}

std::wstring wgetFilenameFile(const std::wstring str) {
	// getFilenameFile file   -- returns: "myImage"
	size_t found;
	std::wstring strt;
	found = str.find_last_of(L"/\\");
	if (found < str.size()) {
		strt = str.substr(found + 1, -1);
		found = strt.find(L".");
		if (found < strt.size()) {
			strt = strt.substr(0, found);
			}
		//return strt;
		}
	else {
		strt = str;
		//return str;
		}
	size_t lastdot = strt.find_last_of(L".");
	if (lastdot == std::wstring::npos) return strt;
	return strt.substr(0, lastdot);
	}

std::wstring wgetFilenameType (std::wstring const &path ) {
	// getFilenameType file -- returns: ".jpg"
	std::wstring ext;
	size_t dotIdx = path.find_last_of( L"." );
	if ( dotIdx != std::wstring::npos ) {
		size_t dirSepIdx = path.find_last_of( L"/\\" );
		if ( dotIdx > dirSepIdx + 1 ) {
			ext = path.substr( dotIdx );
			}
		}
	return ext;
	}


unsigned long SDBMHash(std::string str) {
	// https://www.partow.net/programming/hashfunctions/#SDBMHashFunction
	unsigned long hashid = 0;
	for (unsigned long i = 0; i < str.length(); i++) {
		hashid = ((int)str[i]) + (hashid << 6) + (hashid << 16) - hashid;
		}
	return hashid;
	}

unsigned long DJB2Hash(std::string str) {
	// http://www.cse.yorku.ca/~oz/hash.html
	unsigned long hashid = 5381;
	for (unsigned long i = 0; i < str.length(); i++) {
		hashid = hashid * 33 + ((int)str[i]);
		}
	return hashid;
	}

void playClip (const char &audio_array) {
	// https://stackoverflow.com/questions/22253074/how-to-play-or-open-mp3-or-wav-sound-file-in-c-program
	//PlaySound(audio_array, GetModuleHandle(NULL), SND_MEMORY);
	}

int CustomMessageBox(HWND hWnd = 0, const char* lpText = "TEXT", const char* lpCaption = "Caption", UINT uType = MB_OKCANCEL, UINT uIconResID = 0) {
	// https://forums.codeguru.com/showthread.php?407479-How-to-use-a-custom-icon-in-a-message-box&p=1497200#post1497200
	// need to add linker option -lwinmm
	// MessageBox( 0, "Failed to open in notepad",  "Message", MB_OK + MB_ICONHAND );
	MSGBOXPARAMSA mbp;
	mbp.cbSize = sizeof(MSGBOXPARAMS);
	mbp.hwndOwner = hWnd;
	mbp.hInstance = GetModuleHandle(NULL);
	mbp.lpszText = lpText;
	mbp.lpszCaption = lpCaption;
	mbp.dwStyle = uType | MB_USERICON;
	mbp.dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
	mbp.lpfnMsgBoxCallback = NULL;
	mbp.dwContextHelpId = 0;
	mbp.lpszIcon = MAKEINTRESOURCEA(uIconResID);
	return MessageBoxIndirectA(&mbp);
	}

void decompress_zlib_data (char* &f, size_t fsize, char* &buffer, size_t buffer_size) {
	const int CHUNK = 16384;
	ZLibDecompressor decompressor;
	std::string input = "";
	std::string output = "";
	input.reserve(CHUNK);
	output.reserve(buffer_size);
	for (unsigned long i = 0; i < (fsize / CHUNK); i++) {
		input = std::string(f + ((i * CHUNK)), CHUNK);
		output += decompressor.decompress(input);
		}
	if (fsize - ((fsize / CHUNK) * CHUNK) > 0) {
		input = std::string(f + ((fsize / CHUNK) * CHUNK), fsize - ((fsize / CHUNK) * CHUNK));
		output += decompressor.decompress(input);
		}
	for (unsigned long i = 0; i < buffer_size; i++) {
		if (i < output.length()) {
			buffer[i] = output[i];
			}
		}
	}

//void store_big_files_to_memory (char* &f, size_t fsize)
void convert_mdl_to_smd (fmtBigHeader &files, char* &f, size_t &fsize, std::wstring outfile) {
	/*
	std::string smd = "version 1\n";
	unsigned short game = 0;
	unsigned long hashid;

	// First I need to Decompress every file
	char** data = NULL;
	char** pres = NULL;	// persistent
	char** vset = NULL;	// valuesets
	if (files.count > 0) {
		data = new char*[files.count];
		}
	for (unsigned long i = 0; i < files.count; i++) {
		data[i] = NULL;
		if (files.table[i].raw_size > 0) {
			data[i] = new char[files.table[i].raw_size];
			}
		if (files.table[i].flag == 0) {
			for (unsigned long x = 0; x < files.table[i].raw_size; x++) {
				data[i][x] = f + (files.table[i].addr + x);
				}
			}
		else if (files.table[i].flag == 1) {
			decompress_zlib_data(f + files.table[i].addr, files.table[i].com_size, data[i], files.table[i].raw_size)
			}
		}

	// Determine Game version
	for (unsigned long i = 0; i < files.count; i++) {
		if (files.table[i].name.length() == 11 && files.table[i].name == "SceneHeader") {
			game = readshort(data[i], 4);
			break;
			}
		}

	// build SMD nodes
	smd += "nodes\n";
	for (unsigned long i = 0; i < files.count; i++) {
		smd += NumberToString(i) + " \"";


		if (files.table[i].name == "SceneHeader") {
			smd += "Scene";
			switch (game) {
				case 0x0002: {	// Dead Rising 2 / OTR
					smd += " game:DR2";
					break;
					}
				case 0x0003: {	// Dead Rising 3
					smd += " game:DR3";
					break;
					}
				case 0x0088: {	// Dead RIsing 4
					smd += " game:DR4";
					break;
					}
				}
			smd += "\" -1\n";
			}
		else {

			}


		}
	smd += "end\n";



	for (unsigned long i = 0; i < files.count; i++) {
		// File Could be compressed, send it to the decompressor
		if (files.table[i].raw_size > 0) {
			buffer = new char[files.table[i].raw_size];
			}
		if (files.table[i].flag == 0) {
			for (unsigned long x = 0; x < files.table[i].raw_size; x++) {
				buffer[i] = f + (files.table[i].addr + x);
				}
			}
		else if (files.table[i].flag == 1) {
			decompress_zlib_data(f + files.table[i].addr, files.table[i].com_size, buffer, files.table[i].raw_size)
			}
		else {
			if (buffer != NULL) {
				delete[] buffer;
				buffer = NULL;
				}
			continue;
			}



		// build SMD nodes
		smd += "nodes\n";
		if (files.table[i].name == "SceneHeader") {

			}
		else {

			}


		// Remove File
		if (buffer != NULL) {delete[] buffer; buffer = NULL;}
		}
	writeFile((char*)smd.c_str(), smd.length(), outfile);

	// Clean Up
	if (data != NULL) {
		for (unsigned long i = 0; i < files.count; i++) {
			if (data[i] != NULL) {delete[] data[i];}
			}
		delete[] data;
		}
	*/
	}
void unpack_big (std::wstring wfilename, std::wstring &wfname, std::wstring &wfpath) {
	fmtBigHeader big;
	const int CHUNK = 16384;
	//char inbuf[CHUNK];
	std::string input(CHUNK, '\0');
	std::string output;
	char* f = NULL;
	size_t fsize = 0;
	size_t ptr = 0;

	bool isModel = false;
	// Read file
	if (openFile(f, fsize, wfilename)) {
		ptr = 0;
		big.id = readlong(f, ptr);
		if (big.id == 0x03040506) {
			big.data_addr = readlong(f, ptr);
			big.size = readlong(f, ptr);
			big.count = readlong(f, ptr);
			big.table_addr = readlong(f, ptr);
			big.string_table_addr = readlong(f, ptr);
			if (big.count > 0) {
				big.table = new fmtBigTable[big.count];
				}
			for (unsigned long i = 0; i < big.count; i++) {
				ptr = big.table_addr + (i * 0x1C);
				big.table[i].string_addr = readlong(f, ptr);
				big.table[i].hash_id = readlong(f, ptr);
				big.table[i].com_size = readlong(f, ptr);
				big.table[i].raw_size = readlong(f, ptr);
				big.table[i].addr = readlong(f, ptr);
				big.table[i].unk = readlong(f, ptr);
				big.table[i].flag = readlong(f, ptr);
				ptr = big.table[i].string_addr;
				big.table[i].name = readstring(f, ptr, fsize);

				// Generate a new hash, test if it matches theirs
				big.table[i].hash_sdbm = SDBMHash(lcase(big.table[i].name));
				big.table[i].hash_djb2 = DJB2Hash(lcase(big.table[i].name));

				// Check if one of the files is a Scene Header
				if (isModel == false && big.table[i].name.length() == 11 && big.table[i].name == "SceneHeader") {
					isModel = true;
					}
				}
			if (isModel) {
				convert_mdl_to_smd(big, f, fsize, wfpath + wfname + L".smd");
				}
			else {
				if (big.count > 0) {
					//big.table = new fmtBigTable[big.count];
					_wmkdir((wfpath + wfname).c_str());
					}
				for (unsigned long i = 0; i < big.count; i++) {
					switch (big.table[i].flag) {
						case 0: {		// Not Compressed
							writeFile(f, fsize, wfpath + wfname + L"\\" + utf8_2_unicode(big.table[i].name), big.table[i].addr, big.table[i].com_size);
							break;
							}
						case 1: {		// Compressed
							// Call Decompressor
							//RawInflater inflater(15);
							ZLibDecompressor decompressor;

							// Read file buffer into chunk
							output = "";
							for (unsigned long i = 0; i < ((big.table[i].com_size - 4) / CHUNK); i++) {
								// Convert to string
								input = std::string(f + ((i * CHUNK) + big.table[i].addr + 4), CHUNK);
								//output += inflater.inflate(input);	// without header
								output += decompressor.decompress(input);	// without header
								}
							if (((big.table[i].com_size - 4)) - (((big.table[i].com_size - 4) / CHUNK) * CHUNK) > 0) {
								input = std::string(f + ((((big.table[i].com_size - 4)) / CHUNK) * CHUNK) + big.table[i].addr + 4, ((big.table[i].com_size - 4)) - ((((big.table[i].com_size - 4)) / CHUNK) * CHUNK));
								//input +=  std::string(CHUNK - (fsize - ((fsize / CHUNK) * CHUNK)), '\0');
								//output += inflater.inflate(input);	// without header
								output += decompressor.decompress(input);	// without header
								}

							// WriteFile
							writeFile((char*)output.c_str(), (size_t)(output.size()), wfpath + wfname + L"\\" + utf8_2_unicode(big.table[i].name));
							//writeFile(f, fsize, wfpath + wfname + L"\\" + utf8_2_unicode(big.table[i].name), big.table[i].addr + 6, big.table[i].com_size - 6);
							break;
							}
						default: {
							cout << "unknown flag\n";
							}
						}
					}
				}
			}
		}
	if (f != NULL) {delete[] f; f = NULL;}
	}
void repack_big (std::wstring &wfilename, std::wstring &wfname, std::wstring &wfpath) {

	//char inbuf[CHUNK];

	bool doZlib = false;
	size_t zLibCompressLvl = 7;
	size_t byteAlignment = 4;
	size_t CHUNK = pow(2, zLibCompressLvl) * 128;	// default: 16384
	size_t paddingAmount = 0;
	char* f = NULL;
	size_t fsize = 0;
	size_t ptr = 0;
	std::string input(CHUNK, '\0');
	std::string output;
	// Check if there is an existing file, rename it if there is
	if (PathFileExistsW((wfilename + L".big").c_str())) {
		MoveFileW((wfilename + L".big").c_str(), (wfilename + L"_old.big").c_str());
		}

	// Read all files in the directory
	wDIR* dir;
	DWORD dwAttrib;
	struct wdirent* ent;
	unsigned long numFiles = 0;
	unsigned long totalSizeOfFiles = 0;
	unsigned long totalSizeOfStrings = 0;
	std::wstring wfile;
	std::wstring* dirFiles = NULL;
	if ((dir = wopendir (wfilename.c_str())) != NULL) {

		// Collect File Info
		while ((ent = wreaddir(dir)) != NULL) {
			wfile = wfilename + L"\\" + (std::wstring)ent->d_name;
			dwAttrib = GetFileAttributesW(wfile.c_str());
			if (PathFileExistsW(wfile.c_str()) && dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
				numFiles++;
				totalSizeOfFiles += fileSizeFromAttr(wfile);
				totalSizeOfStrings += ((std::wstring)ent->d_name).length() + 1;
				}
			}
		wclosedir (dir);

		// If there are files
		if (numFiles > 0) {
			// Call compressor
			//ZLibCompressor compressor(4, auto_flush);	// with header
			//RawDeflater deflater;	// without header
			ZLibCompressor compressor;

			// Reserve space in output buffer
			//output.reserve(totalSizeOfFiles + (totalSizeOfFiles * 0x1C) + totalSizeOfStrings + 0x18);
			output = "";

			// store header
			output += "BEEF";	// FileId
			storelong (output, 0x00, 0x03040506);
			output += "BEEF";	// Reserve for Data Addr
			storelong (output, 0x04, 0x18 + (numFiles * 0x1C) + totalSizeOfStrings);
			output += "BEEF";	// Reserve for file Size
			output += "BEEF";	// Reserve for file Count
			storelong (output, 0x0C, numFiles);
			output += "BEEF";	// file table addr
			storelong (output, 0x10, 0x18);
			output += "BEEF";	// Reserve for string buffer addr
			storelong (output, 0x14, 0x18 + (numFiles * 0x1C));

			// store file table
			dirFiles = new std::wstring[numFiles];
			totalSizeOfStrings = 0;
			numFiles = 0;
			dir = wopendir (wfilename.c_str());
			while ((ent = wreaddir(dir)) != NULL) {
				wfile = wfilename + L"\\" + (std::wstring)ent->d_name;
				dwAttrib = GetFileAttributesW(wfile.c_str());
				if (PathFileExistsW(wfile.c_str()) && dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
					dirFiles[numFiles] = (std::wstring)ent->d_name;
					//wcout << numFiles << "\t" << dirFiles[numFiles] << endl;
					numFiles++;
					output += "BEEF";	// Reserve for string addr
					output += "BEEF";	// Reserve for hash
					output += "BEEF";	// Reserve for compressed size
					output += "BEEF";	// Reserve for uncompressed size
					output += "BEEF";	// Reserve for data stream addr
					output += "BEEF";	// Reserve for unknown
					output += "BEEF";	// Reserve for compression flag
					}
				}
			wclosedir (dir);

			// write string buffer
			for (unsigned long i = 0; i < numFiles; i++) {
				storelong (output, 0x18 + (i * 0x1C), output.length());	// store string addr
				storelong (output, 0x1C + (i * 0x1C), SDBMHash(lcase(std::string(dirFiles[i].begin(), dirFiles[i].end()))));	// store hash


				storelong (output, 0x2C + (i * 0x1C), byteAlignment);	// store unknown
				if (doZlib) {
					storelong (output, 0x30 + (i * 0x1C), 0x01);	// store compression flag
					}
				else {
					storelong (output, 0x30 + (i * 0x1C), 0x00);	// store compression flag
					}
				output += std::string(dirFiles[i].begin(), dirFiles[i].end());
				output += '\0';
				}

			paddingAmount = (byteAlignment - (output.length() % byteAlignment)) % byteAlignment;
			for (unsigned long i = 0; i < paddingAmount; i++) {
				output += '\0';	// padding
				}

			// get files in folder
			//cout << numFiles << endl;
			for (unsigned long x = 0; x < numFiles; x++) {
				wfile = wfilename + L"\\" + (std::wstring)dirFiles[x];
				//wcout << dirFiles[x] << endl;
				if (openFile(f, fsize, wfile)) {
					if (fsize > 0) {
						if (doZlib) {	// Compress
							//RawDeflater deflater(zLibCompressLvl, no_flush, 9);	// without header
							ZLibCompressor compressor(zLibCompressLvl, no_flush, 12);


							storelong (output, 0x24 + (x * 0x1C), fsize + 4);	// store uncompressed size


							// Read file buffer into chunk
							ptr = output.length();
							output += "BEEF";	// size
							//output += 0x48;	// unknown
							//output += 0xC7;	// unknown
							for (unsigned long i = 0; i < (fsize / CHUNK); i++) {
								// Convert to string
								input = std::string(f + (i * CHUNK), CHUNK);



								//cout << input << endl;

								// compress
								output += compressor.compress(input);	// with header
								//output += deflater.deflate(input);	// without header
								//output.erase(output.end()-4);
								//output = output.substr(0, output.length() - 4);
								//cout << output << endl;
								}
							if (fsize - ((fsize / CHUNK) * CHUNK) > 0) {
								input = std::string(f + ((fsize / CHUNK) * CHUNK), fsize - ((fsize / CHUNK) * CHUNK));
								//input +=  std::string(CHUNK - (fsize - ((fsize / CHUNK) * CHUNK)), '\0');
								output += compressor.compress(input);	// with header
								//output += deflater.deflate(input);	// without header
								//output.erase(output.end()-4);
								//output = output.substr(0, output.length() - 4);
								}
							//output += deflater.finish();	// without header
							output += compressor.finish();	// without header
							storelong (output, 0x28 + (x * 0x1C), ptr);	// store data stream addr
							storelong (output, 0x20 + (x * 0x1C), output.length() - ptr);	// store compressed size
							storelong (output, ptr, fsize + 4, true);	// store data stream addr
							//storelong (output, ptr, output.length() - ptr, true);	// store data stream addr
							paddingAmount = (byteAlignment - (output.length() % byteAlignment)) % byteAlignment;
							for (unsigned long i = 0; i < paddingAmount; i++) {
								output += '\0';	// padding
								}

							}
						else {	// No Compression
							storelong (output, 0x24 + (x * 0x1C), fsize );	// store uncompressed size
							ptr = output.length();
							for (unsigned long i = 0; i < fsize; i++) {
								//output += f[i];	// without header
								output.append(1, (unsigned char)f[i]);	// without header
								}
							storelong (output, 0x28 + (x * 0x1C), ptr);	// store data stream addr
							storelong (output, 0x20 + (x * 0x1C), output.length() - ptr);	// store compressed size

							for (unsigned long i = 0; i < ((4-((output.length()) % 4)) % 4);i++) {
								output += '\0';	// padding
								}
							}
						}
					else {
						storelong (output, 0x24 + (x * 0x1C), 0);	// store uncompressed size
						storelong (output, 0x28 + (x * 0x1C), output.length());	// store data stream addr
						storelong (output, 0x20 + (x * 0x1C), 0);	// store compressed size
						}
					}
				if (f != NULL) {delete[] f; f = NULL;}
				}
			storelong (output, 0x08, output.length() - 4);

			// WriteFile
			writeFile((char*)output.c_str(), output.size() - 4, wfilename + L".big");
			// Clean up

			//compressor.finish();	// with header

			if (f != NULL) {delete[] f; f = NULL;}
			}
		}
	if (dirFiles != NULL) {delete[] dirFiles; dirFiles = NULL;}





	}
void convert_bct_to_dds (char* bctBuffer, size_t bctSize, char* &ddsBuffer, size_t &ddsSize) {
	// Load BCT Header
	size_t ptr = 0;
	fmtBctHeader bct;
	bct.unk1 = readbyte(bctBuffer, ptr);
	bct.unk2 = readbyte(bctBuffer, ptr);
	bct.unk3 = readbyte(bctBuffer, ptr);
	bct.unk4 = readbyte(bctBuffer, ptr);
	bct.imgWidth = readshort(bctBuffer, ptr);
	bct.imgHeight = readshort(bctBuffer, ptr);
	bct.imgFormat = readbyte(bctBuffer, ptr);
	bct.unk5 = readbyte(bctBuffer, ptr);
	bct.imgMips = readbyte(bctBuffer, ptr);
	bct.imgBitsPerPixel = readbyte(bctBuffer, ptr);
	bct.imgHash = readlong(bctBuffer, ptr);
	bct.imgInfoAddr = readlong(bctBuffer, ptr);
	bct.unk9_count = bct.imgInfoAddr - 0x18;
	if (bct.unk9_count > 0) {
		bct.unk9_buffer = new char[bct.unk9_count];
		}
	for (unsigned long i = 0; i < bct.unk9_count; i++) {
		bct.unk9_buffer[i] = readbyte(bctBuffer, ptr);
		}
	ptr = bct.imgInfoAddr;
	if (bct.imgMips > 0) {
		bct.imgInfo = new fmtBctInfo[bct.imgMips];
		}
	for (unsigned long i = 0; i < bct.imgMips; i++) {	// Mips
		bct.imgInfo[i].dataAddr = readlong(bctBuffer, ptr);
		bct.imgInfo[i].dataSize = readlong(bctBuffer, ptr);	// block size
		bct.imgInfo[i].unk7 = readlong(bctBuffer, ptr);	// flag?, always 0x80000000
		bct.imgInfo[i].unk8 = readlong(bctBuffer, ptr);	// ?, always 0
		//cout << bct.imgInfo[i].dataAddr << "\t" << bct.imgInfo[i].dataSize << "\t" << bct.imgInfo[i].unk7 << endl;
		}
	/*
	// Write BCT Header to a file
	std::string bctx = "";
	bctx += "# This represents data in the BCT Header\n";
	bctx += "# When the BCT is repacked the parameters\n";
	bctx += "# below will be used to write the new BCT\n\n";
	bctx += "imgWidth = " + (NumberToString(bct.imgWidth)) + "\n";
	bctx += "imgHeight = " + (NumberToString(bct.imgHeight)) + "\n";
	bctx += "imgHash = " + (bitIntAsHex(bct.imgHash, 8)) + "\n";
	bctx += "imgFormat = " + (bitIntAsHex(bct.imgFormat, 2)) + "\n";
	//bctx += "imgBits = " + (NumberToString((unsigned long)bct.imgInfo.dataSize)) + "\n";
	bctx += "unk1 = " + (NumberToString((unsigned long)bct.unk1)) + "\n";
	bctx += "unk2 = " + (NumberToString((unsigned long)bct.unk2)) + "\n";
	bctx += "unk3 = " + (NumberToString((unsigned long)bct.unk3)) + "\n";
	bctx += "unk4 = " + (NumberToString((unsigned long)bct.unk4)) + "\n";
	bctx += "unk5 = " + (NumberToString((unsigned long)bct.unk5)) + "\n";
//	bctx += "unk7 = " + (NumberToString((unsigned long)bct.unk7)) + "\n";
	//bctx += "unk8 = " + (bitIntAsHex(bct.imgInfo.unk8, 8)) + "\n";
	bctx += "unk9 = {";

	for (unsigned long i = 0; i < bct.unk9_count; i++) {
		bctx += NumberToString((unsigned long)bct.unk9_buffer[i]);
		if (i < bct.unk9_count - 1) {
			bctx += ", ";
			}
		}

	bctx += "}";
	writeFile((char*)bctx.c_str(), bctx.length(), wfpath + wfname + L".bctx");
	*/


	// Create DDS Header
	img_dds dds;

	// Translate Format ID
	DXGI_FORMAT dds_fmt = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	switch (bct.imgFormat) {
		case 0x00: {dds_fmt = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM; break;}	// "ARGB32"			0		bpp: W x H x4(32)
		case 0x08: {dds_fmt = DXGI_FORMAT::DXGI_FORMAT_BC1_UNORM; break;}		// "BC1"			8		bpp: W x H x0.5(4)		bc1_unorm
		case 0x0A: {dds_fmt = DXGI_FORMAT::DXGI_FORMAT_BC3_UNORM; break;}		// "BC3"			10		bpp: W x H x1 (8)		bc3_unorm
		case 0x25: {dds_fmt = DXGI_FORMAT::DXGI_FORMAT_BC4_UNORM; break;}		// "BC4"			37		bpp: W x H x0.5(4)		bc4_unorm
		case 0x26: {dds_fmt = DXGI_FORMAT::DXGI_FORMAT_BC5_UNORM; break;}		// "BC5U"			38		bpp: W x H x1 (8)		bc5_unorm
		case 0x27: {dds_fmt = DXGI_FORMAT::DXGI_FORMAT_BC6H_UF16; break;}		// "BC6"			39		bpp: W x H x1 (8)		bc6h_uf16
		case 0x28: {dds_fmt = DXGI_FORMAT::DXGI_FORMAT_BC7_UNORM; break;}		// "BC7"			40		bpp: W x H x1 (8)		bc7_unorm
		case 0x30: {dds_fmt = DXGI_FORMAT::DXGI_FORMAT_BC1_UNORM; break;}		// "DXT1"			48		bpp: W x H x0.5(4)
		case 0x32: {dds_fmt = DXGI_FORMAT::DXGI_FORMAT_BC3_UNORM; break;}		// "DXT5"			50		bpp: W x H x1 (8)
		case 0x35: {dds_fmt = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM; break;}	// "ARGB32"			53		bpp: W x H x4(32)
		}



	// Set DDS Header
	dds.setHeader(bct.imgWidth, bct.imgHeight, bct.imgMips - 1, dds_fmt);

	// Calculate New Image Size
	size_t new_dds_size = 0;
	size_t new_dds_data_addr = 0;
	size_t dataAddr = 0;

	if (dds.isFormatDX10()) {new_dds_data_addr = 148;} else {new_dds_data_addr = 128;}

	if (bct.imgMips > 0) {
		dataAddr = bct.imgInfo[0].dataAddr;
		new_dds_size = bctSize - dataAddr;
		}
	else {
		for (unsigned long i = 0; i < bct.imgMips; i++) {
			new_dds_size += bct.imgInfo[i].dataSize;
			}
		}

	// Copy contents of Image into new buffer
	//char* f_new = NULL;
	ddsSize = new_dds_size + new_dds_data_addr + ((16-((new_dds_size + new_dds_data_addr) % 16)) % 16);
	//cout << (new_dds_size ) << endl;
	if (new_dds_size > 0) {
		if (ddsBuffer != NULL) {delete[] ddsBuffer;}
		ddsBuffer = new char[ddsSize];
		// >_< fuuuck cant figure out how to get these working, keep getting crashing
		//std::memcpy(f_new + new_dds_data_addr, f + dataAddr, fsize - dataAddr);
		//std::copy(f + dataAddr, fsize - dataAddr, f_new[new_dds_data_addr]);

		// just copy the slow way, err!!
        for (unsigned long i = 0; i < bctSize - dataAddr; i++) {
			ddsBuffer[new_dds_data_addr + i] = bctBuffer[i + dataAddr];
			}
		}

	// Write DDS Header to new buffer
	dds.writeHeader(ddsBuffer);

	// Some types over lap, convert some to dx9 headers
	switch (bct.imgFormat) {
		case 0x25: {	// ATN1
			writelong(ddsBuffer, 0x54, 0x31495441);
			break;
			}
		case 0x26: {	// ATI2
			writelong(ddsBuffer, 0x54, 0x32495441);
			break;
			}
		case 0x30: {	// DXT1
			writelong(ddsBuffer, 0x54, 0x31545844);
			break;
			}
		case 0x32: {	// DXT5
			writelong(ddsBuffer, 0x54, 0x35545844);
			break;
			}
		case 0x00: {	// A8R8G8B8
			writelong(ddsBuffer, 0x54, 0x00000000);
			writelong(ddsBuffer, 0x60, 0x0000FF00);
			writelong(ddsBuffer, 0x64, 0x000000FF);
			writelong(ddsBuffer, 0x68, 0xFF000000);
			writelong(ddsBuffer, 0x6C, 0x00001000);
			break;
			}
		case 0x35: {	// A8R8G8B8
			writelong(ddsBuffer, 0x54, 0x00000000);
			writelong(ddsBuffer, 0x60, 0x0000FF00);
			writelong(ddsBuffer, 0x64, 0x000000FF);
			writelong(ddsBuffer, 0x68, 0xFF000000);
			writelong(ddsBuffer, 0x6C, 0x00001000);
			break;
			}
		}
	}
void convert_dds_to_bct (char* &ddsBuffer, size_t &ddsSize, char* &bctBuffer, size_t &bctSize) {
	// Read The DDS Header
	size_t ptr = 0;
	if (readlong(ddsBuffer, ptr) == 0x20534444) {
		unsigned long dataAddr = 4 + readlong(ddsBuffer, ptr);
		ptr+=4;
		unsigned long ddsWidth = readlong(ddsBuffer, ptr);
		unsigned long ddsHeight = readlong(ddsBuffer, ptr);
		ptr+=8;
		unsigned long ddsMips = readlong(ddsBuffer, ptr);
		ptr+=52;
		unsigned long ddsFormat = readlong(ddsBuffer, ptr);
		if (ddsFormat == 0x30315844) {
			ptr+=40;
			ddsFormat = readlong(ddsBuffer, ptr);
			}
		else if (ddsFormat == 0) {
			ddsFormat = readlong(ddsBuffer, ptr);
			}
		unsigned long imgBpp = 0;
		unsigned long imgFmt = 0;
		switch (ddsFormat) {
			case 0x00000020: {	// ARGB32
				imgBpp = 32;
				imgFmt = 0x35;
				break;
				}
			case 0x31545844: {	// DXT1
				imgBpp = 4;
				imgFmt = 0x30;
				break;
				}
			case 0x35545844: {	// DXT5
				imgBpp = 8;
				imgFmt = 0x32;
				break;
				}
			case 0x31495441: {	// ATI1N
				imgBpp = 4;
				imgFmt = 0x25;
				break;
				}
			case 0x32495441: {	// ATI2N
				imgBpp = 8;
				imgFmt = 0x26;
				break;
				}
			case 0x55344342: {	// BC4
				imgBpp = 4;
				imgFmt = 0x25;
				break;
				}
			case 0x55354342: {	// BC5
				imgBpp = 8;
				imgFmt = 0x26;
				break;
				}
			case 0x0000005F: {	// BC6H UF16
				imgBpp = 8;
				imgFmt = 0x27;
				break;
				}
			case 0x00000062: {	// BC7
				imgBpp = 8;
				imgFmt = 0x28;
				break;
				}
			default: {
				cout << "DDS Type Not Supported\n";
				}
			}
		unsigned long imgPitch = (ddsWidth * ddsHeight * imgBpp) / 8;

		// Calculate Size of New BCT File
		bctSize = 32;
		bctSize += (ddsMips + 1) * 16;
		if (ddsMips > 1) {
			for (unsigned long i = 0; i < ddsMips;i++) {
				//cout << "Mip: " << i << endl;
				//cout << imgBpp << endl;
				//cout << (ddsWidth * ddsHeight) << endl;
				//cout << (pow(2, i)) << endl;
				//cout << ((ddsWidth * ddsHeight) / (pow(2, i))) << endl;
				//cout << (((ddsWidth * ddsHeight) / (pow(2, i))) * imgBpp) << endl;
				//cout << ((((ddsWidth * ddsHeight) / (pow(2, i))) * imgBpp) / 8) << endl;
				bctSize += (((ddsWidth * ddsHeight) / (pow(2, i))) * imgBpp) / 8;
				}
			}
		else {bctSize += imgPitch; ddsMips = 1;}

		// Create New BCT File
		bctBuffer = new char[bctSize];

		// Write BCT Header
		ptr = 0;
		writelong(bctBuffer, ptr, 0x20020107); ptr+=4;
		writeshort(bctBuffer, ptr, ddsWidth); ptr+=2;
		writeshort(bctBuffer, ptr, ddsHeight); ptr+=2;
		bctBuffer[ptr] = imgFmt; ptr++;
		bctBuffer[ptr] = 255; ptr++;
		bctBuffer[ptr] = ddsMips; ptr++;
		bctBuffer[ptr] = imgBpp; ptr++;
		unsigned long hashid = 0;
		for (unsigned long i = ddsSize; i --> dataAddr; ) {
			hashid = ((unsigned long)ddsBuffer[i]) + (hashid << 6) + (hashid << 16) - hashid;
			}
		writelong(bctBuffer, ptr, hashid); ptr+=4;
		writelong(bctBuffer, ptr, 0x00000020); ptr+=4;
		writelong(bctBuffer, ptr, 0); ptr+=4;
		writelong(bctBuffer, ptr, 0); ptr+=4;
		writelong(bctBuffer, ptr, 0x00010020); ptr+=4;
		unsigned long mipAddr = 32 + (ddsMips * 16);
		unsigned long mipPitch;
		for (unsigned long i = 0; i < ddsMips; i++) {
			mipPitch = (((ddsWidth * ddsHeight) / (pow(2, i))) * imgBpp) / 8;
			writelong(bctBuffer, ptr, mipAddr); ptr+=4;
			writelong(bctBuffer, ptr, mipPitch); ptr+=4;
			writelong(bctBuffer, ptr, 0x80000000); ptr+=4;
			writelong(bctBuffer, ptr, 0); ptr+=4;
			mipAddr += mipPitch;
			}

		// Copy Image Data From DDS to BCT
		ptr = 32 + (ddsMips * 16);
		for (unsigned long i = 0; i < ddsSize - dataAddr; i++) {
			if (ptr + i < bctSize) {
				bctBuffer[ptr + i] = ddsBuffer[dataAddr + i];
				}

			}

		// Write BCT
		//writeFile(bctBuffer, bctSize, wfpath + wfname + L".bct");

		}
	}
void unpack_bct (std::wstring &wfilename, std::wstring &wfname, std::wstring &wfpath) {
	// Load File into Buffer
	char* f = NULL;
	char* f_new = NULL;
	size_t fsize = 0;
	size_t f_new_size = 0;
	openFile(f, fsize, wfilename);

	convert_bct_to_dds(f, fsize, f_new, f_new_size);

	// Write New Complete DDS File
	writeFile(f_new, f_new_size, wfpath + wfname + L".dds");

	/* OLD CODE USING Dev Image Library
	// Load BCT Image Data
	//Initialization of DevIL
	ilInit();
	ILuint ImgId = 0;
	ilGenImages(1, &ImgId);
	ilBindImage(ImgId);


	bool success = ilLoadImage("G:\\SteamLibrary\\steamapps\\common\\DeadRising4\\_Tools\\mcBigPax\\tex_003.tga");
	ilSave(IL_TGA, "G:\\SteamLibrary\\steamapps\\common\\DeadRising4\\_Tools\\mcBigPax\\bin\\Release\\test.tga");
	cout << success<< endl;

	ilBindImage(0);
	ilDeleteImage(ImgId);

	//(gli::load(f + bct.imgInfo.dataAddr, fsize - bct.imgInfo.dataAddr));
	//img_dds ddsfile;
	//ddsfile.setHeader(file.width, file.height, 1, file.type);
	//ddsfile.writeHeader(OutFile);
	//gli::save(TextureSource, "G:\\SteamLibrary\\steamapps\\common\\DeadRising4\\_Tools\\mcBigPax\\bin\\Release\\test.dds");


	*/


	// Clean Up
	if (f != NULL) {delete[] f;}
	if (f_new != NULL) {delete[] f_new;}
	}
void repack_bct (std::wstring wfilename, std::wstring &wfname, std::wstring &wfpath) {
	// Open DDS File
	char* ddsBuffer = NULL;
	size_t ddsSize = 0;
	char* bctBuffer = NULL;
	size_t bctSize = 0;
	openFile(ddsBuffer, ddsSize, wfilename);

	convert_dds_to_bct(ddsBuffer, ddsSize, bctBuffer, bctSize);

	writeFile(bctBuffer, bctSize, wfpath + wfname + L".bct");

    // Clean Up
    if (bctBuffer != NULL) {delete[] bctBuffer;}
    if (ddsBuffer != NULL) {delete[] ddsBuffer;}
	}
void unpack_tex (std::wstring wfilename, std::wstring &wfname, std::wstring &wfpath) {
	fmtBigHeader big;
	fmtBigTable tbl;
	const int CHUNK = 16384;
	//char inbuf[CHUNK];
	std::string input(CHUNK, '\0');
	std::string output;
	char* f = NULL;
	char* d = NULL;
	size_t dsize = 0;
	size_t fsize = 0;
	size_t ptr = 0;
	// Read file
	if (openFile(f, fsize, wfilename)) {
		ptr = 0;
		big.id = readlong(f, ptr);
		if (big.id == 0x03040506) {
			big.data_addr = readlong(f, ptr);
			big.size = readlong(f, ptr);
			big.count = readlong(f, ptr);
			big.table_addr = readlong(f, ptr);
			big.string_table_addr = readlong(f, ptr);

			if (big.count > 0) {
				//big.table = new fmtBigTable[big.count];
				_wmkdir((wfpath + wfname).c_str());
				}
			for (unsigned long i = 0; i < big.count; i++) {
				ptr = big.table_addr + (i * 0x1C);
				tbl.string_addr = readlong(f, ptr);
				tbl.hash_id = readlong(f, ptr);
				tbl.com_size = readlong(f, ptr);
				tbl.raw_size = readlong(f, ptr);
				tbl.addr = readlong(f, ptr);
				tbl.unk = readlong(f, ptr);
				tbl.flag = readlong(f, ptr);
				ptr = tbl.string_addr;
				tbl.name = readstring(f, ptr, fsize);
				if (tbl.raw_size == 0) {continue;}
				switch (tbl.flag) {
					case 0: {		// Not Compressed
						d = new char[tbl.raw_size];



						//writeFile(f, fsize, wfpath + wfname + L"\\" + utf8_2_unicode(tbl.name), tbl.addr, tbl.raw_size);

						convert_bct_to_dds (f + tbl.addr, tbl.raw_size, d, dsize);

						writeFile(d, dsize, wfpath + wfname + L"\\" + utf8_2_unicode(tbl.name) + L".dds");


						if (d != NULL) {delete[] d; d = NULL;}
						break;
						}
					case 1: {		// Compressed
						// Call Decompressor
						RawInflater inflater(15);

						// Read file buffer into chunk
						output = "";
						for (unsigned long i = 0; i < ((tbl.com_size - 6) / CHUNK); i++) {
							// Convert to string
							input = std::string(f + ((i * CHUNK) + tbl.addr + 6), CHUNK);
							output += inflater.inflate(input);	// without header
							}
						if (((tbl.com_size - 6)) - (((tbl.com_size - 6) / CHUNK) * CHUNK) > 0) {
							input = std::string(f + ((((tbl.com_size - 6)) / CHUNK) * CHUNK) + tbl.addr + 6, ((tbl.com_size - 6)) - ((((tbl.com_size - 6)) / CHUNK) * CHUNK));
							//input +=  std::string(CHUNK - (fsize - ((fsize / CHUNK) * CHUNK)), '\0');
							output += inflater.inflate(input);	// without header
							}

						// WriteFile

						d = new char[output.size() - 4];


						convert_bct_to_dds ((char*)output.c_str(), (size_t)(output.size() - 4), d, dsize);



						//writeFile((char*)output.c_str(), (size_t)(output.size() - 4), wfpath + wfname + L"\\" + utf8_2_unicode(tbl.name));
						writeFile(d, dsize, wfpath + wfname + L"\\" + utf8_2_unicode(tbl.name) + L".dds");
						//writeFile(f, fsize, wfpath + wfname + L"\\" + utf8_2_unicode(tbl.name), tbl.addr + 6, tbl.com_size - 6);

						if (d != NULL) {delete[] d; d = NULL;}
						break;
						}
					default: {
						cout << "unknown flag\n";
						}
					}
				}
			}
		}
	if (f != NULL) {delete[] f; f = NULL;}
	}
void repack_tex (std::wstring &wfilename, std::wstring &wfname, std::wstring &wfpath) {
	const int CHUNK = 16384;
	//char inbuf[CHUNK];
	std::string input(CHUNK, '\0');
	std::string output;
	char* f = NULL;
	char* f_new = NULL;
	size_t fsize = 0;
	size_t f_new_size = 0;
	size_t ptr = 0;
	// Check if there is an existing file, rename it if there is
	if (PathFileExistsW((wfilename + L".tex").c_str())) {
		MoveFileW((wfilename + L".tex").c_str(), (wfilename + L"_old.tex").c_str());
		}

	// Read all files in the directory
	wDIR* dir;
	DWORD dwAttrib;
	struct wdirent* ent;
	unsigned long numFiles = 0;
	unsigned long totalSizeOfFiles = 0;
	unsigned long totalSizeOfStrings = 0;
	std::wstring wfile;
	std::wstring wtype;
	std::wstring wname;
	std::wstring* dirFiles = NULL;
	if ((dir = wopendir (wfilename.c_str())) != NULL) {

		// Collect File Info
		while ((ent = wreaddir(dir)) != NULL) {
			wfile = wfilename + L"\\" + (std::wstring)ent->d_name;

			wtype = wlcase(wgetFilenameType(ent->d_name));


			dwAttrib = GetFileAttributesW(wfile.c_str());
			if (wtype.find(L".dds") != std::wstring::npos && PathFileExistsW(wfile.c_str()) && dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
				numFiles++;
				totalSizeOfFiles += fileSizeFromAttr(wfile);
				wname = wgetFilenameFile(wfile);
				totalSizeOfStrings += wname.length() + 1;
				}
			}
		wclosedir (dir);

		// If there are files
		if (numFiles > 0) {
			// Call compressor
			//ZLibCompressor compressor(4, auto_flush);	// with header
			RawDeflater deflater;	// without header

			// Reserve space in output buffer
			//output.reserve(totalSizeOfFiles + (totalSizeOfFiles * 0x1C) + totalSizeOfStrings + 0x18);
			output = "";

			// store header
			output += "BEEF";	// FileId
			storelong (output, 0x00, 0x03040506);
			output += "BEEF";	// Reserve for Data Addr
			storelong (output, 0x04, 0x18 + (numFiles * 0x1C) + totalSizeOfStrings);
			output += "BEEF";	// Reserve for file Size
			output += "BEEF";	// Reserve for file Count
			storelong (output, 0x0C, numFiles);
			output += "BEEF";	// file table addr
			storelong (output, 0x10, 0x18);
			output += "BEEF";	// Reserve for string buffer addr
			storelong (output, 0x14, 0x18 + (numFiles * 0x1C));

			// store file table
			dirFiles = new std::wstring[numFiles];
			totalSizeOfStrings = 0;
			numFiles = 0;
			dir = wopendir (wfilename.c_str());
			while ((ent = wreaddir(dir)) != NULL) {
				wfile = wfilename + L"\\" + (std::wstring)ent->d_name;
				dwAttrib = GetFileAttributesW(wfile.c_str());
				wtype = wlcase(wgetFilenameType(ent->d_name));
				if (wtype.find(L".dds") != std::wstring::npos && PathFileExistsW(wfile.c_str()) && dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
					dirFiles[numFiles] = (std::wstring)ent->d_name;
					//wcout << numFiles << "\t" << dirFiles[numFiles] << endl;
					numFiles++;
					output += "BEEF";	// Reserve for string addr
					output += "BEEF";	// Reserve for hash
					output += "BEEF";	// Reserve for compressed size
					output += "BEEF";	// Reserve for uncompressed size
					output += "BEEF";	// Reserve for data stream addr
					output += "BEEF";	// Reserve for unknown
					output += "BEEF";	// Reserve for compression flag
					}
				}
			wclosedir (dir);

			// write string buffer
			for (unsigned long i = 0; i < numFiles; i++) {
				storelong (output, 0x18 + (i * 0x1C), output.length());	// store string addr
				storelong (output, 0x1C + (i * 0x1C), SDBMHash(lcase(std::string(dirFiles[i].begin(), dirFiles[i].end()))));	// store hash


				storelong (output, 0x2C + (i * 0x1C), 0x04);	// store unknown
				storelong (output, 0x30 + (i * 0x1C), 0x01);	// store compression flag
				wname = wgetFilenameFile(dirFiles[i]);
				output += std::string(wname.begin(), wname.end());
				output += '\0';
				}
			for (unsigned long i = 0; i < ((4-((output.length()) % 4)) % 4);i++) {
				output += '\0';	// padding
				}

			// get files in folder
			//cout << numFiles << endl;
			for (unsigned long x = 0; x < numFiles; x++) {
				wfile = wfilename + L"\\" + (std::wstring)dirFiles[x];
				//wcout << dirFiles[x] << endl;
				if (openFile(f, fsize, wfile)) {
					if (fsize > 0) {


						convert_dds_to_bct(f, fsize, f_new, f_new_size);




						RawDeflater deflater(4, auto_flush, 15);	// without header
						storelong (output, 0x24 + (x * 0x1C), f_new_size + 4);	// store uncompressed size


						// Read file buffer into chunk
						ptr = output.length();
						output += "BEEF";	// size
						output += 0x48;	// unknown
						output += 0xC7;	// unknown
						for (unsigned long i = 0; i < (f_new_size / CHUNK); i++) {
							// Convert to string
							input = std::string(f_new + (i * CHUNK), CHUNK);



							//cout << input << endl;

							// compress
							//output = compressor.compress(input);	// with header
							output += deflater.deflate(input);	// without header
							//output.erase(output.end()-4);
							//output = output.substr(0, output.length() - 4);
							//cout << output << endl;
							}
						if (f_new_size - ((f_new_size / CHUNK) * CHUNK) > 0) {
							input = std::string(f_new + ((f_new_size / CHUNK) * CHUNK), f_new_size - ((f_new_size / CHUNK) * CHUNK));
							//input +=  std::string(CHUNK - (fsize - ((fsize / CHUNK) * CHUNK)), '\0');

							output += deflater.deflate(input);	// without header
							//output.erase(output.end()-4);
							//output = output.substr(0, output.length() - 4);
							}
						output += deflater.finish();	// without header
						storelong (output, 0x28 + (x * 0x1C), ptr);	// store data stream addr
						storelong (output, 0x20 + (x * 0x1C), output.length() - ptr);	// store compressed size
						storelong (output, ptr, f_new_size + 4, true);	// store data stream addr
						//storelong (output, ptr, output.length() - ptr, true);	// store data stream addr
						for (unsigned long i = 0; i < ((4-((output.length()) % 4)) % 4);i++) {
							output += '\0';	// padding
							}
						}
					else {
						storelong (output, 0x24 + (x * 0x1C), 0);	// store uncompressed size
						storelong (output, 0x28 + (x * 0x1C), output.length());	// store data stream addr
						storelong (output, 0x20 + (x * 0x1C), 0);	// store compressed size
						}



					}




				if (f_new != NULL) {delete[] f_new; f_new = NULL;}
				if (f != NULL) {delete[] f; f = NULL;}




				}


			storelong (output, 0x08, output.length() - 4);

			// WriteFile
			writeFile((char*)output.c_str(), output.size() - 4, wfilename + L".tex");
			// Clean up

			//compressor.finish();	// with header

			if (f_new != NULL) {delete[] f_new; f_new = NULL;}
			if (f != NULL) {delete[] f; f = NULL;}
			}
		}
	if (dirFiles != NULL) {delete[] dirFiles; dirFiles = NULL;}





	}

bool doesDirContainDDS (std::wstring &folder) {
	wDIR* dir;
	std::string file;
	std::wstring wfile;
	struct wdirent* ent;
	if ((dir = wopendir (folder.c_str())) != NULL) {
		while ((ent = wreaddir(dir)) != NULL) {
			wfile = wgetFilenameType(wlcase(ent->d_name));;
			file = lcase(std::string(wfile.begin(), wfile.end()));
			if (file.find(".dds") != std::wstring::npos) {
				wclosedir (dir);
				return true;
				}
			}
		wclosedir (dir);
		}
	return false;
	}

bool processFile (std::wstring wfilename) {
	std::wstring wfpath = wgetFilenamePath(wfilename);
	std::wstring wfname = wgetFilenameFile(wfilename);
	//wcout << wfilename << endl;
	// check if input is a folder or a file
	if (wfilename != L"." && wfilename != L".." && PathIsDirectoryW(wfilename.c_str())) {
		// directory, REPACK
		if (!doesDirContainDDS(wfilename)) {
			PlaySoundA(array_theres_a_party, GetModuleHandle(NULL), SND_MEMORY);
			repack_big(wfilename, wfname, wfpath);
			}
		else {
			PlaySoundA(array_yaa_now_were_talkin, GetModuleHandle(NULL), SND_MEMORY);
			repack_tex(wfilename, wfname, wfpath);
			}
		}
	else {
		// File, UNPACK
		// Check file type
		std::wstring wftype = wlcase(wgetFilenameType(wfilename));
		//wcout << wftype << endl;
		if (wftype.find(L".big") != std::string::npos) {
			//PlaySound(array_time_to_go, GetModuleHandle(NULL), SND_MEMORY);
			PlaySoundA(array_biting_chip, GetModuleHandle(NULL), SND_MEMORY);
			unpack_big(wfilename, wfname, wfpath);
			}
		else if (wftype.find(L".mdl") != std::string::npos) {
			//PlaySound(array_time_to_go, GetModuleHandle(NULL), SND_MEMORY);
			PlaySoundA(array_biting_chip, GetModuleHandle(NULL), SND_MEMORY);
			unpack_big(wfilename, wfname, wfpath);
			}
		else if (wftype.find(L".tex") != std::string::npos) {
			//PlaySound(array_time_to_go, GetModuleHandle(NULL), SND_MEMORY);
			PlaySoundA(array_theres_a_party, GetModuleHandle(NULL), SND_MEMORY);
			unpack_tex(wfilename, wfname, wfpath);
			}
		else if (wftype.find(L".bct") != std::string::npos) {
			PlaySoundA(array_time_to_go, GetModuleHandle(NULL), SND_MEMORY);
			unpack_bct(wfilename, wfname, wfpath);
			}
		else if (wftype.find(L".dds") != std::string::npos) {
			PlaySoundA(array_right_on, GetModuleHandle(NULL), SND_MEMORY);
			repack_bct(wfilename, wfname, wfpath);
			}
		else {
			PlaySoundA(array_damn_batteries_dead, GetModuleHandle(NULL), SND_MEMORY | SND_ASYNC);
			CustomMessageBox(0, "File Type Is Not Supported", "!! W A R N I N G !!", MB_OK, IDI_ICON2);
			}
		}
	return true;
	}

int main(int argc, char* argv[]) {
	std::wstring wfilename;

	// Check for input
	if (argc < 2) {
		// No Input, request file
		wfilename = openfilenameW(
			L"All Supported Files\0*.big;*.mdl;*.smd;*.tex;*.bct;*.dds\0\
			Archive Files (*.big)\0*.big\0\
			Model Files (*.mdl, *.smd)\0*.big;*.smd\0\
			Texture Files (*.tex, *.bct, *.dds)\0*.tex;*.bct;*.dds\0\
			All Files (*.*)\0*.*\0"
			);
		if (wfilename.length() == 0) {
			PlaySoundA(array_good_bye, GetModuleHandle(NULL), SND_MEMORY);
			return 0;
			}
		if (!PathFileExistsW(wfilename.c_str())) {return 0;}
		processFile(wfilename);
		}
	else {
		// Loop Through Inputs
		for (signed long x = 1; x < argc; x++) {
			// convert filename to unicode
			wfilename = utf8_2_unicode(argv[x]);

			// check that path can be accessed
			if (PathFileExistsW(wfilename.c_str())) {
				processFile(wfilename);
				}
			}
		}
	//system("pause");
    return 0;
	}
