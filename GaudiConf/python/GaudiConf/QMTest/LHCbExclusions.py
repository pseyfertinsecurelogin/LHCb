from GaudiTest import normalizeExamples, RegexpReplacer, LineSkipper

preprocessor = normalizeExamples + \
    RegexpReplacer(when = "Connected to database",
        orig = r'ONLINE[_-][0-9 ]{1,6}(.*)"[^"]*/([0-9A-Z_]{1,8})"',
        repl = r'ONLINE_xxxxxx\1"\2"') + \
    RegexpReplacer(when = "Connected to database",
        orig = r'"[^"]*/([0-9A-Z_]{1,8})"',
        repl = r'"\1"') + \
    LineSkipper(["/PARAM/ParamFiles/"]) + \
    LineSkipper(["INFO Using TAG", "TimingAuditor.T", "RootDBase.open"]) + \
    LineSkipper(["INFO Opened magnetic field file"]) + \
    LineSkipper(["INFO Connecting to database"]) + \
    LineSkipper(["INFO Disconnect from database after being idle"]) + \
    LineSkipper(["INFO  'CnvServices':"])
