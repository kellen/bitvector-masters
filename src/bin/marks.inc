                switch(benchnum) {
                    case 0:
                        s = new  BoolNoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 1:
                        s = new BitHCS7NoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 2:
                        s = new BitDecompNoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 3:
                        s = new BitHCNoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 4:
                        s = new SetNoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 5:
                        s = new BitHCIntNoneRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 6:
                        s = new BitHCIntDegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 7:
                        s = new BitHCDegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 8:
                        s = new BitHCActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 9:
                        s = new BitHCIntActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 10:
                        s = new BitHCS7DegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 11:
                        s = new BitHCS7ActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 12:
                        s = new BitDecompDegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 13:
                        s = new SetDegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 14:
                        s = new SetActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 15:
                        s = new SetRndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 16:
                        s = new BitHCS7RndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 17:
                        s = new BitHCRndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 18:
                        s = new BitHCIntRndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 19:
                        s = new BitDecompRndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 20:
                        s = new BitDecompActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 21:
                        s = new  BoolDegreeRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 22:
                        s = new  BoolActivityRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    case 23:
                        s = new  BoolRndRnd(&(skip[64*runnum]), num_skip[skipidx]);
                        break;
                    default:
                        done = true;
                }
