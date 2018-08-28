class IPlayer
{
public:
	virtual void SetSnakeNumber(size_t snakeNumber) {}
	virtual size_t GetSnakeNumber() { return 0; }
};