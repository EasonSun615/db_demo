db_demo : main.cpp InputBuffer.cpp InputBuffer.h MetaCommand.cpp MetaCommand.h Row.cpp Row.h Statement.cpp Statement.h Table.cpp Table.h Pager.cpp Pager.h Cursor.cpp Cursor.h Node.cpp Node.h LeafNode.cpp LeafNode.h InternalNode.cpp InternalNode.h
	g++ -o db_demo main.cpp InputBuffer.cpp MetaCommand.cpp Row.cpp Statement.cpp Table.cpp Pager.cpp Cursor.cpp LeafNode.cpp Node.cpp InternalNode.cpp
