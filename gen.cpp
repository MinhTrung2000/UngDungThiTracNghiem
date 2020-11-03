///**
// *  This code is used to generate random data for example
// */
//
//#include <bits/stdc++.h>
//#include "control/lib_utility.h"
//using namespace std;
//
//const char* DIR_LOGIN_DATA_FILE = "data/LOGIN.data";
//const char* DIR_DSMON_DATA_FILE = "data/DANH_SACH_MON_HOC.data";
//const char* DIR_DSLOP_DATA_FILE = "data/DANH_SACH_LOP.data";
//const char* DIR_DSCAUHOITHI_DATA_FILE = "data/CAUHOITN.data";
//
//const int MAX_ACCOUNT_GV = 3;
//
//const string ACC_GIAOVIEN[MAX_ACCOUNT_GV][2] = {
//	{"TRAN VAN A", "GV0001"},
//	{"NGUYEN VAN B", "GV0002"},
//	{"DO THI C", "GV0003"}
//};
//
//const int MAX_ACCOUNT_SV = 10;
//
//const string ACC_SINHVIEN[MAX_ACCOUNT_SV][2] = {
//	{"TRUONG HONG TUAN", "SV0001"},
//	{"NGUYEN THI MAI", "SV0002"},
//	{"DAO HONG TUYEN", "SV0003"},
//	{"TRAN CHI TAM", "SV0004"},
//	{"NGUYEN HONG PHUC", "SV0005"},
//	{"TRUONG QUANG DUNG", "SV0006"},
//	{"NGUYEN THI LE TRINH", "SV0007"},
//	{"BUI TUAN PHUOC", "SV0008"},
//	{"NGUYEN HONG HOA", "SV0009"},
//	{"HO GIA HUY", "SV0010"},
//};
//
//bool gen_Login() {
//	cout << "Create file LOGIN.data...";
//	fstream fw(DIR_LOGIN_DATA_FILE);
//	if (Utility::check_existed_file(fw) && !Utility::check_empty_file(fw)) {
//		cout << "\n! File 'LOGIN.data' is already existed.\n";
//		return false;
//	}
//	fw.open(DIR_LOGIN_DATA_FILE, ios::out);
//	fw << MAX_ACCOUNT_GV << '\n';
//	for (int i = 0; i < MAX_ACCOUNT_GV; ++i) {
//		fw << ACC_GIAOVIEN[i][0] << '\n' << ACC_GIAOVIEN[i][1] << '\n';
//	}
//	fw << MAX_ACCOUNT_SV << '\n';
//	for (int i = 0; i < MAX_ACCOUNT_SV; ++i) {
//		fw << ACC_SINHVIEN[i][0] << '\n' << ACC_SINHVIEN[i][1] << '\n';
//	}
//	fw.close();
//	cout << "completely\n";
//	return true;
//}
//
//const int MAX_MONHOC = 5;
//
//const string MONHOC[MAX_MONHOC][2] = {
//	{"CAU TRUC DU LIEU GIAI THUAT", "CTDLGT0001"},
//	{"LY THUYET THONG TIN", "LTTT0001"},
//	{"DAI SO TUYEN TINH", "DSTT0001"},
//	{"HE DIEU HANH", "HDH0001"},
//	{"MANG MAY TINH", "MMT0001"}
//};
//
//bool gen_DSMonHoc() {
//	cout << "Create file DANH_SACH_MON_HOC.data...";
//	fstream fw(DIR_DSMON_DATA_FILE);
//	if (Utility::check_existed_file(fw) && !Utility::check_empty_file(fw)) {
//		cout << "\n! File 'DANH_SACH_MON_HOC.data' is already existed.\n";
//		return false;
//	}
//	fw.open(DIR_DSMON_DATA_FILE, ios::out);
//	fw << MAX_MONHOC << '\n';
//	for (int i = 0; i < MAX_MONHOC; ++i) {
//		fw << MONHOC[i][0] << '\n' << MONHOC[i][1] <<'\n';
//	}
//	fw.close();
//	cout << "completely\n";
//	return true;
//}
//
//const int MAX_LOPHOC = 5;
//
//const string LOPHOC[MAX_LOPHOC][2] = {
//	{"A01", "Cong Nghe Thong Tin A01"},
//	{"A02", "Cong Nghe Thong Tin A02"},
//	{"A03", "Cong Nghe Thong Tin A03"},
//	{"A04", "Cong Nghe Thong Tin A04"},
//	{"A05", "Cong Nghe Thong Tin A05"},
//};
//
//bool gen_DSLop() {
//	cout << "Create file DANH_SACH_LOP.data...";
//	fstream fw(DIR_DSLOP_DATA_FILE);
//	if (Utility::check_existed_file(fw) && !Utility::check_empty_file(fw)) {
//		cout << "\n! File 'DANH_SACH_LOP.data' is already existed.\n";
//		return false;
//	}
//	fw.open(DIR_DSLOP_DATA_FILE, ios::out);
//	fw << MAX_LOPHOC << '\n';
//	for (int i = 0; i < MAX_LOPHOC; ++i) {
//		fw << LOPHOC[i][0] << '\n' << LOPHOC[i][1] << '\n';
//		int x = rand() % MAX_ACCOUNT_SV;
//		int y = rand() % (MAX_ACCOUNT_SV - x) + x;
//		fw << (y - x + 1) << '\n';
//		for (int i = x; i <= y; ++i) {
//			fw << ACC_SINHVIEN[i][0] << '\n' << ACC_SINHVIEN[i][1] << '\n';
//		}
//	}
//	fw.close();
//	cout << "completely\n";
//	return true;
//}
//
////int main() {
////	ios::sync_with_stdio(false);
////	cin.tie(0);
////
////	srand(time(NULL));
////
////	if (gen_Login() == false) return 0;
////	if (gen_DSMonHoc() == false) return 0;
////	if (gen_DSLop() == false) return 0;
////
////	return 0;
////}
