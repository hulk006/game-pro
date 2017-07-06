package ai

import ("card"
		"hand"
		"fmt"
		"dealmachine"
		"sync"
		"math/rand"
		"time"
)

type AI struct{
	chip int               //剩余筹码
	hole card.Cards
	communitycards card.Cards
	com_size int
	h *hand.Hand

	mutex *sync.Mutex
}

func GetAI() *AI{
	ai := new(AI)
	ai.h = hand.GetHand()
	ai.hole = make(card.Cards, 2, 2)
	ai.communitycards = make(card.Cards, 5, 5)
	ai.mutex = new(sync.Mutex)
	ai.chip = 5000
	return ai
}

func (ai *AI)Init(){
	ai.com_size = 0
	ai.h.Init()
}

func (ai *AI)ShowHand(){
	ai.h.ShowHand()
}
//手牌2
func (ai *AI)SetHole(c1 *card.Card, c2 *card.Card){
	ai.hole[0] = c1
	ai.hole[1] = c2
	ai.h.SetCard(c1)
	ai.h.SetCard(c2)
}
//翻三张牌第一次3张工牌
func (ai *AI)SetFlop(c1 *card.Card, c2 *card.Card, c3 *card.Card){
	ai.communitycards[ai.com_size] = c1
	ai.com_size++
	ai.communitycards[ai.com_size] = c2
	ai.com_size++
	ai.communitycards[ai.com_size] = c3
	ai.com_size++
	ai.h.SetCard(c1)
	ai.h.SetCard(c2)
	ai.h.SetCard(c3)
}
//翻一张牌第二次翻牌4
func (ai *AI)SetTurn(c1 *card.Card){
	ai.communitycards[ai.com_size] = c1
	ai.com_size++
	ai.h.SetCard(c1)
}
//翻一张牌第三次次翻牌5
func (ai *AI)SetRiver(c1 *card.Card){
	ai.communitycards[ai.com_size] = c1
	ai.com_size++
	ai.h.SetCard(c1)
}
//分析手牌
func (ai *AI)DealOver(){
	ai.h.AnalyseHand()
}
//下注分析
func (ai *AI)FCR(bet int, gambpool int, dm *dealmachine.DealMachine) int{  //return 0 mean fold
	strength := ai.getStrength(dm)
	if (ai.chip-bet)<400 && strength<0.5{
		return 0
	}
	if strength>=0.9{
		return ai.chip
	}
//第一轮下注的时候没有回报率，设置为1.3，然后按着回报会下注
	if bet==0 {
		if strength < 0.5{
			return 0
		}
		RR := 1.3
		RR -= strength
		res := (strength*float64(gambpool))/RR
		return int(res)
	}else {
		//RR计算回报率
		RR := strength*(float64(bet)+float64(gambpool))/float64(bet)
		fmt.Printf("RR %f, strength %f\n", RR, strength)
		source := rand.NewSource(time.Now().UnixNano())
		r := rand.New(source)
		p := r.Int()%100
		if strength>0.7 && p>=30{
			return bet
		}
		if RR <0.8 {
			if p >= 5{
				return 0
			}else {
				return bet*2
			}
		}else if RR>=0.8 && RR<1.0 {
			if p>=20 {
				return 0
			}else if p<5 {
				return bet
			}else if p>=5 && p<20 {
				return 2*bet
			}
		}else if RR>=1.0 && RR<1.3 {
			if p>=40 {
				return bet
			}else{
				return bet*2
			}
		}else if RR>=1.3 {
			if p>=70 {
				return bet
			}else {
				return bet*2
			}
		}
	}
	return 0
}

func (ai *AI)getStrength(dm *dealmachine.DealMachine) float64{
	
	threadnum := 50
	c := make(chan int, threadnum)
	for i:=0; i<threadnum; i++{
		ddm, err := dm.CopyTheDm()
		if err != nil{
			fmt.Println(err)
			return 0
		}
		go simulate(ai, ddm, c)
	}

	wintime := 0
	count := 0
	for {
		if count == threadnum{
			break
		}
		t := <-c
		count++
		wintime += t
	}

	return float64(wintime)/(float64(threadnum)*100)
}

func simulate(ai *AI, dm *dealmachine.DealMachine, c chan int){

	h1 := hand.GetHand()
	h2 := hand.GetHand()

	count := 0
	for i:=0; i<100; i++{
		dm.Shuffle()

		h1.Init()
		h2.Init()

		h1.SetCard(ai.hole[0])
		h1.SetCard(ai.hole[1])

		h2.SetCard(dm.Deal())
		h2.SetCard(dm.Deal())
//如果有公牌
		for j:=0; j<ai.com_size; j++{
			h1.SetCard(ai.communitycards[j])
			h2.SetCard(ai.communitycards[j])
		}
//如果没有公牌
		for j:=0; j<(5-ai.com_size); j++{
			cc := dm.Deal()
			h1.SetCard(cc)
			h2.SetCard(cc)
		}

		h1.AnalyseHand()
		h2.AnalyseHand()

		if h1.Level > h2.Level{
			count++
		}else if h1.Level == h2.Level && h1.FinalValue > h2.FinalValue{
			count++
		}
	}
	c <- count	
}

func (ai *AI)ShowChip(){
	fmt.Printf("电脑本金还剩：%d\n",ai.chip)
}

func (ai *AI) ShowHole(){
	ai.h.ShowHole()
}

var RANKNAME = []string{"2","3","4","5","6","7","8","9","10","J","Q","K","A"}
var SUITNAME = []string{"黑桃", "红桃", "梅花", "方块"}
func (ai *AI)ShowComminityCards(){
	fmt.Printf("目前场上公牌为:\n")
	for i:=0; i<ai.com_size; i++{
		fmt.Printf("%s %s     ", SUITNAME[ai.communitycards[i].Suit], RANKNAME[ai.communitycards[i].Value])
	}
	fmt.Println()
}

func (ai *AI)GetChip() int{
	return ai.chip
}

func (ai *AI)GetLevel() int{
	return ai.h.Level
}

func (ai *AI)GetFinalValue()int{
	return ai.h.FinalValue
}

func (ai *AI)Blind(bet int){
	ai.chip -= bet
}

func (ai *AI)Take(bet int){
	ai.chip += bet
}

func (ai *AI)Call(bet int){
	ai.chip -= bet
}
