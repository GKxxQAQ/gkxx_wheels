#include "wheels/dynarray.hpp"
#include "wheels/matrix.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string>

// width of normal column and the row index column
const int DEFAULT_COLUMN_WIDTH = 7;
const int DEFAULT_ROW_IDX_WIDTH = 4;

class NumberSpreadSheetCell;
class StringSpreadSheetCell;

enum CellType { Number, String };

class SpreadSheetCell {
    friend std::ostream &operator<<(std::ostream &, const SpreadSheetCell &);

public:
    SpreadSheetCell(CellType t) : m_type(t) {}
    virtual ~SpreadSheetCell() {}
    virtual bool operator<(const SpreadSheetCell &) const = 0;
    CellType GetType() const {
        return m_type;
    }
    virtual const std::string getString() const = 0;
    virtual const double getValue() const = 0;

protected:
    CellType m_type;
};

std::ostream &operator<<(std::ostream &os, const SpreadSheetCell &ss) {
    if (ss.GetType() == CellType::String)
        os << ss.getString();
    else
        os << std::setprecision(3) << ss.getValue();
    return os;
}

class NumberSpreadSheetCell : public SpreadSheetCell {
    double value;

public:
    NumberSpreadSheetCell(double v)
        : SpreadSheetCell(CellType::Number), value(v) {}
    virtual bool operator<(const SpreadSheetCell &) const override;
    virtual const std::string getString() const override {
        return "";
    }
    virtual const double getValue() const override {
        return value;
    }
};

bool NumberSpreadSheetCell::operator<(const SpreadSheetCell &that) const {
    if (that.GetType() == CellType::String)
        return true;
    return value < that.getValue();
}

class StringSpreadSheetCell : public SpreadSheetCell {
    std::string str;

public:
    StringSpreadSheetCell(const std::string &s)
        : SpreadSheetCell(CellType::String), str(s) {}
    virtual bool operator<(const SpreadSheetCell &) const override;
    virtual const std::string getString() const override {
        return str;
    }
    virtual const double getValue() const override {
        return 0;
    }
};

bool StringSpreadSheetCell::operator<(const SpreadSheetCell &that) const {
    if (that.GetType() == CellType::Number)
        return false;
    return str < that.getString();
}

class SpreadSheet {
public:
    SpreadSheet(int rows, int cols);
    ~SpreadSheet();
    void HideRow(int row) {
        row_valid[row - 1] = false;
    }
    void HideColumn(int col) {
        col_valid[col - 1] = false;
    }
    void ShowRow(int row) {
        row_valid[row - 1] = true;
    }
    void ShowColumn(int col) {
        col_valid[col - 1] = true;
    }
    void UpdateCell(int row, int col, SpreadSheetCell *cell) {
        --row;
        --col;
        if (m_cells[row][col] && m_cells[row][col] != cell)
            delete m_cells[row][col];
        m_cells[row][col] = cell;
    }
    void ExportSheet(std::ostream &os) const;
    void SortByColumn(int);

private:
    gkxx::Matrix<SpreadSheetCell *> m_cells;
    gkxx::Dynarray<bool> row_valid, col_valid;
};

class ColumnComparator {
    int col;

public:
    ColumnComparator(int c) : col(c) {}
    bool operator()(const gkxx::Dynarray<SpreadSheetCell *> &lhs,
                    const gkxx::Dynarray<SpreadSheetCell *> &rhs) {
        if (!lhs[col])
            return false;
        return !rhs[col] || *lhs[col] < *rhs[col];
    }
};

inline void SpreadSheet::SortByColumn(int c) {
    std::sort(m_cells.begin(), m_cells.end(), ColumnComparator(c - 1));
}

SpreadSheet::~SpreadSheet() {
    for (const auto &v : m_cells)
        for (auto x : v)
            delete x;
}

SpreadSheet::SpreadSheet(int rows, int cols)
    : m_cells(rows, cols, nullptr), row_valid(rows, true),
      col_valid(cols, true) {}

void SpreadSheet::ExportSheet(std::ostream &os) const {
    os << std::string(DEFAULT_ROW_IDX_WIDTH, ' ') + "|";
    int col_cnt = 0;
    if (!m_cells.empty()) {
        for (std::size_t i = 0; i < m_cells[0].size(); ++i)
            if (col_valid[i]) {
                os << std::setw(DEFAULT_COLUMN_WIDTH) << i + 1;
                ++col_cnt;
            }
    }
    os << std::endl;
    os << std::string(
              DEFAULT_ROW_IDX_WIDTH + DEFAULT_COLUMN_WIDTH * col_cnt + 1, '-')
       << std::endl;
    for (std::size_t i = 0; i < m_cells.size(); ++i)
        if (row_valid[i]) {
            os << std::setw(DEFAULT_ROW_IDX_WIDTH) << i + 1 << "|";
            if (!m_cells.empty()) {
                for (std::size_t j = 0; j < m_cells[0].size(); ++j)
                    if (col_valid[j]) {
                        os << std::setw(DEFAULT_COLUMN_WIDTH);
                        if (m_cells[i][j])
                            os << *m_cells[i][j];
                        else
                            os << "";
                    }
                os << std::endl;
            }
        }
}

void test() {
    SpreadSheet ss(5, 5);
    ss.UpdateCell(1, 1, new NumberSpreadSheetCell(1.1));
    ss.UpdateCell(1, 3, new NumberSpreadSheetCell(3.4));
    ss.UpdateCell(2, 4, new StringSpreadSheetCell("foo"));
    ss.UpdateCell(3, 1, new StringSpreadSheetCell("bar"));
    ss.UpdateCell(3, 2, new StringSpreadSheetCell("baz"));
    ss.UpdateCell(4, 1, new NumberSpreadSheetCell(0.5));
    ss.UpdateCell(4, 2, new NumberSpreadSheetCell(0));
    ss.UpdateCell(5, 1, new StringSpreadSheetCell("raz"));
    ss.SortByColumn(1);
    ss.ExportSheet(std::cout);
}

int main() {
    test();
}