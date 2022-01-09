
function G = make_gen(H)
    m = size(H, 1);
    n = size(H, 2);
    k = n - m;
    F = make_gen_(H);
    G = [eye(k), F];
end

function F = make_gen_(H)    
    m = size(H, 1);
    n = size(H, 2);
    k = n - m;
    A = H(1:m, 1:k);
    B = H(1:m, k+1:n);
    F = transpose(A) * inv(transpose(B)); 
end

