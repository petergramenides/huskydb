#include "huskydb.h"

using namespace std;

int main()
{
	// Starting database testing
	cout << "[HuskyDB] Starting database verification test." << endl;

	// Initialize database
	CHuskyDB db = CHuskyDB("/path/to/db");

	// Make package in database
	db.MakePackage("test");

	// Make table in database
	db.MakeTable("test", "test");

	// Check that table exists
	CTable* tbl = db.QueryTable("test", "test");
	cout << "[HuskyDB] Table exists as " << tbl->GetName() << endl;

	// Delete table from database
	db.DeleteTable("test", "test");

	// Check that package exists
	using namespace std::chrono;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	CPackage* pkg = db.QueryPackage("test");
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();
	cout << "[HuskyDB] query time: ~" << duration << " ms" << endl;
	cout << "[HuskyDB] Package exists as " << pkg->GetName() << endl;
	cout << "[HuskyDB] Package contains tables: " << pkg->GetTables().size() << endl;

	// Delete package from database
	db.DeletePackage("test");

	// Check that items exist
	CPackage* pkg2 = db.QueryPackage("package");
	cout << "[HuskyDB] Package exists as " << pkg2->GetName() << endl;
	cout << "[HuskyDB] Package contains tables: " << pkg2->GetTables().size() << endl;
	cout << "[HuskyDB] Package contains tables: " << pkg2->GetTables()[0]->GetName() << endl;
	cout << "[HuskyDB] Package contains files: " << pkg2->GetTables()[0]->GetFiles()[0]->GetName() << endl;


	// Delete file from database
	//db.DeleteFile("package", "images", "test.png");

	cin.get();
}