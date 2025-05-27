class logic
{
public:
	void setup();
	bool set_x(int x, int y);
	bool set_o(int x, int y);
	void done(bool &tie, bool &xwon, bool &owon);
private:
	//a multilevel array for the logical representation of a 3x3 board
	char board[3][3];
};

