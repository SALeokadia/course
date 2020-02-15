class Solution {
public:
    int numRookCaptures(vector<vector<char>>& board) {
        int count = 0, x = -1, y = -1, i, j;
        for(i = 0; i < 8; ++i)
        {
        	for(j = 0; j < 8; ++j)
        		if(board[i][j] == 'R')
    			{
    				x = i, y = j;
    				break;
    			}
			if(x != -1)
				break;
        }
        bt(board, count, x, y-1, 0);
        bt(board, count, x, y+1, 1);
        bt(board, count, x-1, y, 2);
        bt(board, count, x+1, y, 3);
        return count;
    }
    void bt(vector<vector<char>> &board, int &count, int x, int y, int dir)
    {
    	if(x < 0 || y < 0 || x > 7 || y > 7 || board[x][y] == 'B')
    		return;
		if(board[x][y] == '.')
		{
			if(dir == 0)
				bt(board, count, x, y-1, 0);
			else if(dir == 1)
				bt(board, count, x, y+1, 1);
			else if(dir == 2)
        		bt(board, count, x-1, y, 2);
        	else
        		bt(board, count, x+1, y, 3);
    	}
    	else
    	{
    		++count;
    	} 	
    }
};