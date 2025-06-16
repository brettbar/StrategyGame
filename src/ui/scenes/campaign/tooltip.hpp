namespace UI {
  struct Tooltip {

    static Tooltip *Get() {
      static Tooltip instance;
      return &instance;
    }

private:
    Tooltip() {}
    ~Tooltip() {}

    Tooltip(Tooltip const &) = delete;
    void operator=(const Tooltip &) = delete;
  };
}// namespace UI
