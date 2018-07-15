#include "huskydb.h"

using namespace std;

int main()
{
	// Starting database testing
	cout << "[HuskyDB] Starting database verification test." << endl;

	// Initialize database
	huskydb db = huskydb("/path/to/db");

	// Make package in database
	db.make_package("test");

	// Make table in database
	db.make_table("test", "test");

	// Check that table exists
	table* tbl = db.query_table("test", "test");
	cout << "[HuskyDB] Table exists as " << tbl->get_name() << endl;

	// Delete table from database
	db.delete_table("test", "test");

	// Check that package exists
	using namespace std::chrono;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	package* pkg = db.query_package("test");
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();
	cout << "[HuskyDB] query time: ~" << duration << " ms" << endl;
	cout << "[HuskyDB] Package exists as " << pkg->get_name() << endl;
	cout << "[HuskyDB] Package contains tables: " << pkg->get_tables().size() << endl;

	// Delete package from database
	db.delete_package("test");

	// Check that items exist
	package* pkg2 = db.query_package("package");
	cout << "[HuskyDB] Package exists as " << pkg2->get_name() << endl;
	cout << "[HuskyDB] Package contains tables: " << pkg2->get_tables().size() << endl;
	cout << "[HuskyDB] Package contains tables: " << pkg2->get_tables()[0]->get_name() << endl;
	cout << "[HuskyDB] Package contains files: " << pkg2->get_tables()[0]->get_files()[0]->get_name() << endl;


	// Delete file from database
	db.delete_file("package", "images", "test.png");

	cin.get();
}
