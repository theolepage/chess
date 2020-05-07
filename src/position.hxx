namespace board
{
    inline Position::Position(File file, Rank rank)
        : file_(file)
        , rank_(rank)
    {}

    inline bool Position::operator==(const Position& pos) const
    {
        return file_get() == pos.file_get() && rank_get() == pos.rank_get();
    }

    inline bool Position::operator!=(const Position& pos) const
    {
        return !(*this == pos);
    }

    inline File Position::file_get() const
    {
        return file_;
    }

    inline Rank Position::rank_get() const
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

} // namespace board
