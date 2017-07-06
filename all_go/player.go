/*
术语
德州扑克术语大全
Action 叫注 / 说话 一个玩家的决定共有七种。

Bet 押注 - 押上筹码

Call 跟进 - 跟随众人押上同等的注额

Fold 收牌 / 不跟 - 放弃继续牌局的机会

raise 加注 - 把现有的注金抬高

Re-raise 再加注 - 再别人加注以后回过来再加注

All-in 全押 - 一次过把手上的筹码全押上
Pot 底池 - 每一个牌局里众人已押上的筹码总额,也即该局的奖金数目。
Pot Odds: 底池中的金额与您跟注所需金额的比率。"底池比率"越大,您跟注所需金额越多
*/
package player

import ("hand"
		"card"
		"fmt"
)

type Player struct{
	h *hand.Hand
	chip int
	hole card.Cards
}

func GetPlayer() *Player{
	p := new(Player)
	p.h = hand.GetHand()
	p.chip = 5000
	p.hole = make(card.Cards, 2, 2)
	return p
}

func (p *Player)Init(){
	p.h.Init()
}

func (p *Player)Blind(bet int){
	p.chip -= bet
}

func (p *Player)Call(bet int){
	p.chip -= bet
}

func (p *Player)Take(bet int){
	p.chip += bet
}

func (p *Player)SetHole(c1 *card.Card, c2 *card.Card){
	p.hole[0] = c1
	p.hole[1] = c2
	p.h.SetCard(c1)
	p.h.SetCard(c2)
}

func (p *Player)SetFlop(c1 *card.Card, c2 *card.Card, c3 *card.Card){
	p.h.SetCard(c1)
	p.h.SetCard(c2)
	p.h.SetCard(c3)
}

func (p *Player)SetTurn(c1 *card.Card){
	p.h.SetCard(c1)
}

func (p *Player)SetRiver(c1 *card.Card){
	p.h.SetCard(c1)
}

func (p *Player)DealOver(){
	p.h.AnalyseHand()
}

func (p *Player)ShowChip(){
	fmt.Printf("你的本金还剩：%d\n", p.chip)
}

var RANKNAME = []string{"2","3","4","5","6","7","8","9","10","J","Q","K","A"}
var SUITNAME = []string{"黑桃", "红桃", "梅花", "方块"}
func (p *Player) ShowHole(){
	if p.hole[0] == nil || p.hole[1] == nil{
		return
	}
	fmt.Printf("你的手牌为：%s %s、%s %s\n",SUITNAME[p.hole[0].Suit], RANKNAME[p.hole[0].Value], SUITNAME[p.hole[1].Suit], RANKNAME[p.hole[1].Value])
}

func (p *Player)GetChip() int{
	return p.chip
}

func (p *Player)GetLevel() int{
	return p.h.Level
}

func (p *Player)GetFinalValue()int{
	return p.h.FinalValue
}
