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

} // namespace board
