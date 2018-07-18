## Introduction
HuskyDB is a lightweight document-oriented database optimized for Windows :)
It's written entirely in C++ and has no external dependencies. The target is
small applications and video games that do not need large databases.

### Database Structure

HuskyDB was designed with a very specific data management model. Many NoSQL databases are very unstructured, leading to increased query times. HuskyDB adopts a hybrid model, we utilize a custom hierarchical structure to maximize query speeds and minimize transition from
conventional RDBMS SQL-based databases.

<img src="https://github.com/petergramenides/huskydb/blob/master/img/huskydb-structure.JPG?raw=true" width="800">

## HuskyDB API Functions

### Constructors

Description: Initializes the NoSQL database from the provided path
stored as a string primitive. If the database location does
not exist, the database will thrown an error and exit.

```
// Initialize NoSQL database
CHuskyDB(string input_path);
```

Description: Initializes the NoSQL database from the provided path
stored as a string primitive. If the database location does
not exist, the database will thrown an error and exit.

The database will also take a boolean value `toggle_pr` as
the second argument, which enables prioritization of
different files. These values can be set in the `priority`
files generate after you start the database in the file
directories.

```
// Initialize NoSQL database (with prioritization setting)
CHuskyDB(string input_path, bool toggle_pr);
```

### Indexing

Description: Indexes the packages, tables, and files in the database and stores them into different class structures: CPackage, CTable, and CFile.

Argument: The boolean value of `toggle_pr`, which enables priotiization of different files in the database.

```
// Index database in memory
void					Index(bool toggle_pr);
```

### Querying

Description: Returns a package if its name matches that of the provided `package_name` variable. Throws `nullptr` if no match is found.

Argument: Name of the package you want to find represented by a string primitive.

```
// Find package by name
CPackage*				QueryPackage(string package_name);
```

Description: Returns a table if its name matches that of the provided `table_name` variable. 

Argument: Name of the table you want to find represented by a string primitive.

Behavior: Throws `nullptr` if no match is found. This function will throw the first table found with the given name. If you can have multiple tables with the same name or are operating a dynamic database, use the overloaded function below so there will not be duplicates.
```
// Find table through table name
CTable*					QueryTable(string table_name);
```

Description: Returns a table if its name matches that of the provided `table_name` variable and the `package_name` variable.

Arguments: Name of the table and package you want to find represented by string primitives.

Behavior: Throws `nullptr` if no match is found. This function will throw a `CTable*` if found. There cannot be duplicates in the database for a table if it has a corresponding package name since there cannot be duplicate package names or table names within a package.

```
// Find table through multiple parameters
CTable*					QueryTable(string table_name, string package_name);
```

### Make Elements

Description: Makes a package folder and returns a boolean value representing the status of the package.

Arguments: The name of the package you want to create represented as a string primitive.

Behavior: Returns true if success, false if fails to create package. There cannot be duplicate packages and this function will regulate that behavior.

```
// Make package
bool					MakePackage(string package_name);
```

Description: Makes a table folder within a package and retuns a boolean value representing the status of the table.

Arguments: The name of the package you want the table to be created in and the name of the table represented as string primitives.

Behavior: Returns true if success, false if fails to create table. There cannot be duplicate table names within a package and this function will regulate that behavior.

```
// Make table
bool					MakeTable(string package_name, string table_name);
```

Description: Makes a file entry within a table and retuns a boolean value representing the status of the file.

Arguments: The name of the package and table you want the file to be contained in as well as the name of the file, the contents of the file represented as a string, and the priority value (set to zero if priority is disabled).

Behavior: Returns true if success, false if fails to create file. There cannot be duplicate file names within a package with the same extension and this function will regulate that behavior.

```
// Make priority file from string
bool					MakeFile(string package_name, string table_name, string file_name, string data, size_t priority);
```

### Append File
Description: Appends to a file entry within a table and retuns a boolean value representing the status of the file.

Arguments: The name of the package and table the file is contained in as well as the name of the file and the contents you wish to append to the file represented as a vector of strings.

Behavior: Returns true if success, false if fails to append to the file.

```
// Append data to file from string (can be used for large files)
bool					AppendFile(string package_name, string table_name, string file_name, vector<string> data);
```

### Delete Elements

Description: Deletes a package and retuns a boolean value representing the status of the deletion.

Arguments: The name of the package you want to delete represented as a string primitive.

Behavior: Returns true if success, false if fails to delete the package. Deletes the package as well as all tables and files within the package.

```
// Delete package if it exists including all content (tables, files, etc.)
bool					DeletePackage(string package_name);
```

Description: Delete a table with a package and retuns a boolean value representing the status of the deletion.

Arguments: The name of the package that keeps the table and the name of the table you wish to delete represented as string primitives.

Behavior: Returns true if success, false if fails to delete the table. Deletes the table as well as all files within the table.

```
// Delete table if it exists including all content
bool					DeleteTable(string package_name, string table_name);
```

Description: Delete a file entry contained within a table and package and retuns a boolean value representing the status of the deletion.

Arguments: The name of the package and table that keep the file as well as the name of the file represented as string primitives.

Behavior: Returns true if success, false if fails to delete the file.

```
// Delete individual file
bool					DeleteFile(string package_name, string table_name, string file_name);
```
