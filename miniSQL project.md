# miniSQL project

## command-line argument

`./minidb input.sql output.txt`

## Syntax of miniSQL

1. `CREATE DATABASE database_name`

2. ` USE DATABASE database_name` 

3. ` CREATE TABLE table_name ( column1_name column1_type, column2_name column2_type, column3_name column3_type, ... );`

    In this project, minidb will support a simplified CREATE TABLE command with three data types: FLOAT, TEXT, and INTEGER. This command allows users to define a table by specifying its name and columns. Each column must have a unique name and one of the three supported data types.

4. ` DROP TABLE table_name`

5. ` INSERT INTO table_name VALUES (value1, value2, ...)`

6. `SELECT column1, column2, ... FROM table_name`

   `SELECT * FROM table_name`

7. ` SELECT column1, column2 FROM table_name WHERE condition1 AND/OR condition2`

   Comparisons: column > value, column < value, column = value

   Logical Connectors: AND and OR to combine multiple conditions

8. `SELECT table1.column1, table2.column1 FROM table1 INNER JOIN table2 ON table1.X = table2.Y;`

9. `UPDATE table_name SET column1 = new_value1, column2 = new_value2, ... WHERE condition;`

10. `DELETEFROMtable_name WHERE condition;`

## Format of the Output

## Error Report