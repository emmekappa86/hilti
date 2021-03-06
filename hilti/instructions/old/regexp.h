iBegin(regexp, Compile, "regexp.compile")
    iOp1(optype::ref\ <regexp>, trueX)
    iOp2(optype::string | ref<list<string>>, trueX)

    iValidate {
        auto ty_op1 = as<type::ref\ <regexp>>(op1->type());
        auto ty_op2 = as<type::string | ref<list<string>>>(op2->type());

    }

    iDoc(R"(    
        Compiles the pattern(s) in *op2* for subsequent matching. *op2* can be
        either a string with a single pattern, or a list of strings with a set
        of patterns. All patterns must only contain ASCII characters and must
        not contain any back-references.  Each pattern instance can be
        compiled only once. Throws ~~ValueError if a second compilation
        attempt is performed.  Todo: We should support other than ASCII
        characters too but need the notion of a local character set first.
    )")

iEnd

iBegin(regexp, Find, "regexp.find")
    iTarget(optype::int\ <32>)
    iOp1(optype::ref\ <regexp>, trueX)
    iOp2(optype::string | iterator<bytes>, trueX)
    iOp3(optype::[None | iterator<bytes>], trueX)

    iValidate {
        auto ty_target = as<type::int\ <32>>(target->type());
        auto ty_op1 = as<type::ref\ <regexp>>(op1->type());
        auto ty_op2 = as<type::string | iterator<bytes>>(op2->type());
        auto ty_op3 = as<type::[None | iterator<bytes>]>(op3->type());

    }

    iDoc(R"(    
        Scans either the string in *op1* or the byte iterator range between
        *op2* and *op3* for the regular expression *op1* (if op3 is not given,
        searches until the end of the bytes object). Returns a positive
        integer if a match found found; if a set of patterns has been
        compiled, the integer then indicates which pattern has matched. If
        multiple patterns from the set match, the left-most one is taken. If
        multiple patterns match at the left-most position, it is undefined
        which of them is returned. The instruction returns -1 if no match was
        found but adding more input bytes could change that (i.e., a partial
        match). Returns 0 if no match was found and adding more input would
        not change that.  Todo: The string variant is not yet implemented.
    )")

iEnd

iBegin(regexp, Groups, "regexp.groups")
    iTarget(optype::ref\ <vector\ <(iterator<bytes>,iterator<bytes>)>>)
    iOp1(optype::ref\ <regexp>, trueX)
    iOp2(optype::string | iterator<bytes>, trueX)
    iOp3(optype::None | iterator<bytes>, trueX)

    iValidate {
        auto ty_target = as<type::ref\ <vector\ <(iterator<bytes>,iterator<bytes>)>>>(target->type());
        auto ty_op1 = as<type::ref\ <regexp>>(op1->type());
        auto ty_op2 = as<type::string | iterator<bytes>>(op2->type());
        auto ty_op3 = as<type::None | iterator<bytes>>(op3->type());

    }

    iDoc(R"(    
        Scans either the string in *op1* or the byte iterator range between
        *op2* and *op3* for the regular expression op1*. If the regular
        expression is found, returns a vector with one entry for each group
        defined in the regular expression. Each entry is either the substring
        matching the corresponding subexpression or a range of iterators
        locating the matching bytes, respectively. Index 0 always contains the
        string/bytes that match the total expression. Returns an empty vector
        if the expression is not found.  This method is not compatible with
        sets of multiple patterns; throws PatternError if used with a set, or
        when no pattern has been compiled yet.  Todo: The string variant is
        not yet implemented.
    )")

iEnd

iBegin(regexp, MatchToken, "regexp.match_token")
    iTarget(optype::(int\ <32>,iterator<bytes>))
    iOp1(optype::ref\ <regexp>, trueX)
    iOp2(optype::string | iterator<bytes>, trueX)
    iOp3(optype::[None | iterator<bytes>], trueX)

    iValidate {
        auto ty_target = as<type::(int\ <32>,iterator<bytes>)>(target->type());
        auto ty_op1 = as<type::ref\ <regexp>>(op1->type());
        auto ty_op2 = as<type::string | iterator<bytes>>(op2->type());
        auto ty_op3 = as<type::[None | iterator<bytes>]>(op3->type());

    }

    iDoc(R"(    
        Matches the beginning of either the string in *op1* or the byte
        iterator range between *op2* and *op3* for the regular expression
        *op1* (if op3 is not given, searches until the end of the bytes
        object).  The regexp must have been compiled with the ~~NoSub
        attribute.  Returns a 2-tuple with (1) a integer match-indicator
        corresponding to the one returned by ~~Find; and (2) a bytes iterator
        that pointing one beyond the last examined byte (i.e., right after the
        match if we had one, or right after the input data if not).  Note: As
        the name implies, this a specialized version for parsing purposes,
        enabling optimizing for the case that we don't need any subexpression
        capturing and must start the match right at the initial position.
        Internally, the implementation is only slightly optimized at the
        moment but it could be improved further at some point.  Todo: The
        string variant is not yet implemented. The bytes implementation should
        be further optimized.
    )")

iEnd

iBegin(regexp, MatchTokenInit, "regexp.match_token_advance")
    iTarget(optype::(int\ <32>,iterator<bytes>))
    iOp1(optype::ref\ <match_token_state>, trueX)
    iOp2(optype::string | iterator<bytes>, trueX)
    iOp3(optype::[None | iterator<bytes>], trueX)

    iValidate {
        auto ty_target = as<type::(int\ <32>,iterator<bytes>)>(target->type());
        auto ty_op1 = as<type::ref\ <match_token_state>>(op1->type());
        auto ty_op2 = as<type::string | iterator<bytes>>(op2->type());
        auto ty_op3 = as<type::[None | iterator<bytes>]>(op3->type());

    }

    iDoc(R"(    
        Performs matching previously initialized with
        ~~regexp.match_token_init`` on either the string in *op2* or the byte
        iterator range between *op2* and *op3*. If op3 is not given, searches
        until the end of the bytes object. This method can be called multiple
        times with new data as long as no match has been found, and it will
        continue matching from the previous state as if all data would have
        been concatenated.  Returns a 2-tuple with (1) a integer match-
        indicator corresponding to the one returned by ~~Find; and (2) a bytes
        iterator that pointing one beyond the last examined byte (i.e., right
        after the match if we had one, or right after the input data if not).
        The same match state must not be used again once this instructions has
        returned a match indicator >= zero.  Note: As their name implies, the
        ``regexp.match_token_*`` family of instructions are specialized
        versiond for parsing purposes, enabling optimizing for the case that
        we don't need any subexpression capturing and must start the match
        right at the initial position.  Todo: The string variant is not yet
        implemented.
    )")

iEnd

iBegin(regexp, MatchTokenInit, "regexp.match_token_init")
    iTarget(optype::ref\ <match_token_state>)
    iOp1(optype::ref\ <regexp>, trueX)

    iValidate {
        auto ty_target = as<type::ref\ <match_token_state>>(target->type());
        auto ty_op1 = as<type::ref\ <regexp>>(op1->type());

    }

    iDoc(R"(    
        Initializes incrementatal matching for the regexp *op1*. *op1* will be
        considered implicitly anchored to the beginning of the data, and it
        must have been compiled with the ~~NoSub attribute.  This instruction
        does not perform any matching itself, you use
        ~~regexp.match_token_advance for that.  Note: As their name implies,
        the ``regexp.match_token_*`` family of instructions are specialized
        versiond for parsing purposes, enabling optimizing for the case that
        we don't need any subexpression capturing and must start the match
        right at the initial position.
    )")

iEnd

iBegin(regexp, Span, "regexp.span")
    iTarget(optype::(int\ <32>,(iterator<bytes>,iterator<bytes>)))
    iOp1(optype::ref\ <regexp>, trueX)
    iOp2(optype::string | iterator<bytes>, trueX)
    iOp3(optype::None | iterator<bytes>, trueX)

    iValidate {
        auto ty_target = as<type::(int\ <32>,(iterator<bytes>,iterator<bytes>))>(target->type());
        auto ty_op1 = as<type::ref\ <regexp>>(op1->type());
        auto ty_op2 = as<type::string | iterator<bytes>>(op2->type());
        auto ty_op3 = as<type::None | iterator<bytes>>(op3->type());

    }

    iDoc(R"(    
        Scans either the string in *op1* or the byte iterator range between
        *op2* and *op3* for the regular expression *op1*. Returns a 2-tuple
        with (1) a integer match-indicator corresponding to the one returned
        by ~~Find; and (2) the matching substring or a tuple of iterators
        locating the bytes which match, respectively; if there's no match, the
        second element is either an empty string or a tuple with two
        ``bytes.end`` iterators, respectively.  Throws PatternError if no
        pattern has been compiled yet.  Todo: The string variant is not yet
        implemented.
    )")

iEnd

