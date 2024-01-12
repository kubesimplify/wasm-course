use http::{Method, StatusCode};
use spin_sdk::{
    http::{IntoResponse, Response},
    http_component,
    key_value::Store,
};

#[http_component]
fn handle_pizza(req: http::Request<Vec<u8>>) -> anyhow::Result<impl IntoResponse> {
    let store = Store::open_default()?;

    let (status, body) = match *req.method() {
        Method::POST => {
            store.set(req.uri().path(), req.body().as_slice())?;
            (StatusCode::CREATED, None)
        },
        Method::GET => {
            match store.get(req.uri().path())? {
                Some(order_details) => (StatusCode::OK, Some(order_details)),
                None => (StatusCode::NOT_FOUND, None),
            }
        },
        Method::DELETE => {
            store.delete(req.uri().path())?;
            (StatusCode::OK, None)
        },
        _ => (StatusCode::METHOD_NOT_ALLOWED, None),
    };

    Ok(Response::new(status, body))
}
