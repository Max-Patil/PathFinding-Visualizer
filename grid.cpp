#include "grid.hpp"

bool disable = true;

std::vector<vector<int>> xDir;
std::vector<vector<int>> yDir;

const std::vector<int> xDir4{-1, 1, 0, 0};
const std::vector<int> yDir4{0, 0, -1, 1};

const std::vector<int> xDir8{-1, -1, -1, 0, 0, 1, 1, 1};
const std::vector<int> yDir8{-1, 0, 1, -1, 1, -1, 0, 1};


GridBlock::GridBlock(size_t w, size_t h, size_t x, size_t y, BlockType type) : type(type)
{
    cell.w = w;
    cell.h = h;
    cell.x = x;
    cell.y = y;
}

bool Grid::isValid(int x, int y)
{
    return (x >= 0 && x < gridSize && y >= 0 && y < gridSize);
}

Grid::Grid() : Graph(), connect(Connect::C4)
{
    numNodes = gridSize;
    xDir.push_back(xDir4);
    xDir.push_back(xDir8);
    yDir.push_back(yDir4);
    yDir.push_back(yDir8);
}

void Grid::init()
{
    nodeHeight = windowHeight / numNodes;
    nodeWidth =  windowWidth / numNodes;

    // Initialze gridblock to graph
    int scrVertex = 0, destVertex = 0;
    int newRow = 0, newCol = 0;

    for (int row = 0; row < numNodes; row++)
    {
        for (int col = 0; col < numNodes; col++)
        {
            int xCell = nodeWidth *  row + 3;
            int yCell = nodeHeight * col + 3;

            GridBlock block(nodeWidth - 3, nodeHeight - 3, xCell, yCell, BlockType::BASE);
            scrVertex = row * numNodes + col;

            addVertex(scrVertex, block);

            for (int dir = 0; dir < xDir[connect].size(); dir++)
            {
                newRow = row + xDir[connect][dir];
                newCol = col + yDir[connect][dir];

                if (isValid(newRow, newCol))
                {
                    destVertex = newRow * numNodes + newCol;
                    addEdge(scrVertex, destVertex);
                }
            }
        }
    }
}


GridBlock &Grid::getBlock(int x, int y)
{
    int vertex = x * numNodes + y;
    return getVertex(vertex);
}

GridBlock &Grid::getBlock(VertexId id)
{
    int x = id / numNodes;
    int y = id % numNodes;

    return getBlock(x, y);
}

void Grid::setConnections(Connect connections)
{
    connect = connections;
}

void Grid::changeBlockType(int type, VertexId id)
{
   getBlock(id).type = static_cast<BlockType>(type);
}

bool Grid::handleBlockType(VertexId id)
{
    GridBlock& block = getBlock(id);

    if(block.type == BlockType::BLOCK)
        return true;
    else
        return false;
}
