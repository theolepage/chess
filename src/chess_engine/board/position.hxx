namespace board
{
    inline Position::Position(File file, Rank rank)
        : file_(file)
        , rank_(rank)
    {}

    inline bool Position::operator==(const Position& pos) const
    {
        return get_file() == pos.get_file() && get_rank() == pos.get_rank();
    }

    inline bool Position::operator!=(const Position& pos) const
    {
        return !(*this == pos);
    }

    inline File Position::get_file() const
    {
        return file_;
    }

    inline Rank Position::get_rank() const
    {
        return rank_;
    }

    inline std::optional<Position> Position::move(int file, int rank) const
    {
        int new_file = static_cast<int>(file_) + file;
        int new_rank = static_cast<int>(rank_) + rank;

        if (new_file < 0 || new_rank < 0 || new_file >= 8 || new_rank >= 8)
            return std::nullopt;

        return Position(static_cast<File>(new_file), static_cast<Rank>(new_rank));
    }

    inline std::ostream& operator<<(std::ostream& os, const Position& pos)
    {
        os << "(" << (char)(static_cast<char>(pos.get_file()) + 'A');
        os << ";" << static_cast<int>(pos.get_rank()) + 1 << ")";
        return os;
    }

    inline int get_index(void) const
    {
        return static_cast<int>(file_) + static_cast<int>(rank_) * 8;
    }

} // namespace board
