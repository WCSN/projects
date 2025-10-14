#!/bin/ruby


puts (1+2)

mess01 = 'Доставка необходимых предметов осуществляется во-время.'
mess02 = 'Доставка запчастей осуществляется с перебоями.'
puts 'Я люблю' + 'яблочный пирог.'
puts ( mess01 +' '+ mess02)

puts 'Как Ваше полное имя?'
name = gets.chomp
puts 'Вы знаете, что Ваше имя состоит из '+name.length.to_s+' символов, '+name+'?'
  

lineWidth = 50
puts(  'Вот вам юная мисс из России:'.center(lineWidth))
puts(   'Визг её был ужасен по силе.'.center(lineWidth))
puts(        'Он разил, как кинжал,-'.center(lineWidth))
puts(          'Так никто не визжал,'.center(lineWidth))
puts('Как визжала та Мисс из России.'.center(lineWidth))
  
puts rand
puts rand
puts rand
puts(rand(100))
puts(rand(100))
puts(rand(100))
puts(rand(1))
puts(rand(1))
puts(rand(1))
puts(rand(99999999999999999999999999999999999999999999999999999999999))
puts('Синоптик сказал, что с вероятностью в '+rand(101).to_s+'% пойдёт дождь,')
puts('но никогда не стоит доверять синоптикам.')


puts(Math::PI)
puts(Math::E)
puts(Math.cos(Math::PI/3))
puts(Math.tan(Math::PI/4))
puts(Math.log(Math::E**2))
puts((1 + Math.sqrt(5))/2)


puts 'Привет, как Вас зовут?'

name = gets.chomp
puts 'Привет, ' + name + '.'
if name == 'ASDF'
  puts 'Ваше имя: ' + name
else 
  puts 'Вы ввели имя: ' + name
end

puts 'Привет, как Вас зовут?'
name = gets.chomp
puts 'Привет, ' + name + '.'
if (name == 'ASDF' or name == 'QWER')
  puts 'Какое милое имя!'
end


class Die  #  игральная кость   
  def roll
    1 + rand(6)
  end
  
end

#  Давайте создадим пару игральных костей...
dice = [Die.new, Die.new]

#  ...и бросим их.
dice.each do |die|
  puts die.roll
end
