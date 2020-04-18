db_demo : main.cpp InputBuffer.cpp MetaCommand.cpp Row.cpp Statement.cpp Table.cpp
	g++ -o db_demo main.cpp InputBuffer.cpp MetaCommand.cpp Row.cpp Statement.cpp Table.cpp Pager.cpp Cursor.cpp LeafNode.cpp
