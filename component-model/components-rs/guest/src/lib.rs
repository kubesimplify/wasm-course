use pulldown_cmark::{html, Parser};
use crate::exports::markdown::Markdown;

wit_bindgen::generate!({
    path: "../wit/markdown.wit",
    world: "renderer",
    exports: { "markdown": MyMarkdown },
});

struct MyMarkdown;

impl Markdown for MyMarkdown {
    fn render(input: String) -> String {
        let parser = Parser::new(&input);
        let mut html_output = String::new();
        html::push_html(&mut html_output, parser);
        return html_output;
    }
}
