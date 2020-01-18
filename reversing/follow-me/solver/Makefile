ANSWER := test-input

AUTH := --anyauth --user seccon:Tomor1nHaSuperCute
CURL_HEADER := -H 'Content-Type:application/json'
API_SUBMIT_QUALS_0 := http://$(SERVER)/submit/quals/0

all: solve wait wrong
	@

assert:
	@if [ -z $(SERVER) ]; then echo "[!] Specify \$$SERVER host:port"; exit 1; fi

solve: assert $(ANSWER)
	@echo "[*] Flag"
	curl -s $(CURL_HEADER) -d "{\"input\": \"`cat $(ANSWER)`\"}" $(API_SUBMIT_QUALS_0) | grep "$$(cat ../FLAG)"

$(ANSWER):
	time ./solver.py ../build/trace/calc.trace ../build/sample/calc `python -c "print '?'*200"`

wait:
	@sleep 4

wrong: assert
	@echo "[*] Wrong input"
	curl -s $(CURL_HEADER) -d "{\"input\": \"invalid\"}" $(API_SUBMIT_QUALS_0) | grep "Your input does not follows my execution trace. Hack harder!"

attack-server:
	@ echo "[*] Header Injection"
	curl -s $(CURL_HEADER) -H 'X-Forwarded-For:../' -d "{\"input\": \"invalid\"}" $(API_SUBMIT_QUALS_0)
	sleep 4
	@ echo "[*] Too long input"
	curl -s $(CURL_HEADER) -H 'X-Forwarded-For:../' -d "{\"input\": \"$$(python -c 'print("*"*200)')\"}" $(API_SUBMIT_QUALS_0) | grep error
	sleep 4
	@ echo "[*] Flooding"
	ab -n 1000 -c 10 http://$(SERVER)/

load-test: $(ANSWER)
	echo "{\"input\": \"`cat $<`\"}" > post-data
	ab -n 100 -c 1 -p post-data -T 'application/json' $(API_SUBMIT_QUALS_0)
	rm post-data

clean:
	rm $(ANSWER)